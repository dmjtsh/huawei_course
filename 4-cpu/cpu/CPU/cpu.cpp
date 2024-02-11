#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "cpu.h"
#include "video_memory.h"
#include "../../cpu_source/commands.h"
#include "../../DimasLIB/DimasUtilities/utilities.h"
#include "../../cpu_source/error_processing.h"
#include "../../DimasLIB/DimasStack/stack.h"

void CPUDump(CPU* cpu, size_t num_of_line, FILE* logger)
{
	static size_t num_of_call = 1;
	fprintf(logger, 
		"=======================================\n"
		"CPU DUMP CALL #%zu\n"
		"Line: ", 
		num_of_call);
	if (num_of_line == BEFORE_PROCRESSING_FILE)
		fprintf(logger, "Before processing file\n");
	else
		fprintf(logger, "%zu\n", num_of_line);

	if (cpu->errors)
	{
		fprintf(logger,
			"-------------ERRORS------------\n");
		if (cpu->errors & CPU_PTR_NULL)
		{
			fprintf(logger, "CPU POINTER IS NULL\n");
			return;
		}
		if (cpu->errors & CPU_BAD_STACK)			fprintf(logger, "SOME TROUBLES WITH STACK STRUCT\n");
		if (cpu->errors & CPU_WRONG_INPUT)          fprintf(logger, "WRONG INPUT\n");
		if (cpu->errors & CPU_WRONG_COMMAND_USAGE)  fprintf(logger, "WRONG COMMAND USAGE\n");
		if (cpu->errors & CPU_LOGER_ERROR)          fprintf(logger, "CPU LOGER ERROR\n");
		if (cpu->errors & CPU_CS_PTR_NULL)          fprintf(logger, "CPU CS PTR NULL\n");
		if (cpu->errors & CPU_COMPILED_FILE_ERROR)  fprintf(logger, "CPU COMPILED FILE ERROR\n");		
		CommandDump(&cpu->current_command, logger);

		fprintf(logger,
			"----------END_OF_ERRORS--------\n");
	}
	else
		fprintf(logger,
			"------------NO_ERRORS----------\n");

	if (num_of_line > 0)
	{
		for (size_t current_line = 1; current_line <= cpu->commands_num; current_line++)
		{
			CPUCommand current_cmd_cpu_code     = *(CPUCommand*)((char*)cpu->CS + (current_line-1) * sizeof(CPUCommandWithArg));
			Elem_t     current_cmd_cpu_arg_code = *(Elem_t*)(    (char*)cpu->CS + (current_line-1) * sizeof(CPUCommandWithArg) + 8);

			fprintf(logger, "(%2zu) %3d " ELEM_T_IDENTIFIER, current_line, current_cmd_cpu_code, current_cmd_cpu_arg_code);
			if (current_line == cpu->current_line_num+2)
				fprintf(logger, " <-----");
			fprintf(logger, "\n");
		}
	}
	#define REG_DEF(name, code, ...) fprintf(logger, "%s:" ELEM_T_IDENTIFIER ", " , #name);
	
	#include "../../cpu_source/regs_defs.h"

	#undef REG_DEF

	fprintf(logger, "R[0]: %lg, R[1]: %lg\n" , cpu->RAM[0], cpu->RAM[1]);

	fprintf(logger, "\n");
	CommandDump(&cpu->current_command, logger);
	fprintf(logger,
		"=======================================\n\n");
	
	num_of_call++;
}

int CPUVerifier(CPU* cpu)
{
	if (!cpu)
		return CPU_PTR_NULL;

	StackCheckState(&cpu->stack);

	CHECK_ERROR(cpu, cpu->stack.errors,         CPU_BAD_STACK)
	CHECK_ERROR(cpu, !cpu->logger,              CPU_LOGER_ERROR)
	CHECK_ERROR(cpu, !cpu->compiled_file,       CPU_COMPILED_FILE_ERROR)
	CHECK_ERROR(cpu, cpu->current_command.error,CPU_CURRENT_COMMAND_ERROR)
	CHECK_ERROR(cpu, !cpu->CS,                  CPU_CS_PTR_NULL)

	return cpu->errors;
}

bool IsValidNumArg(Command* command)
{
	assert(command != NULL);

	if(command->CPU_cmd_with_arg.cmd == POP && !(command->cmd_arg_type & MEMORY_TYPE))
	{
		command->error = POP_WITH_NUM;
		return false;
	}
	return true;
}

bool IsValidRegArg(Command* command)
{
	assert(command != NULL);

	bool is_reg_found = false;

	switch((size_t)command->CPU_cmd_with_arg.arg)
	{
		#define REG_DEF(name, cpu_code, ...) \
		case cpu_code:                       \
			is_reg_found = true;             \
			break;

		#include "../../cpu_source/regs_defs.h"
	}

	#undef REG_DEF
	
	if(!is_reg_found)
	{
		command->error = INVALID_REG_OR_LABEL_NAME;
		return false;
	}

	return true;
}

bool IsValidCommand(Command* command)
{
	assert(command != NULL);

	if (!command)
		return false;

	if ((char)command->CPU_cmd_with_arg.cmd & (MEMORY_TYPE | NUMBER_TYPE | REGISTER_TYPE) 
		&& command->CPU_cmd_with_arg.cmd != HLT)
	{
		command->arguments_num = 1;
		if((short)command->CPU_cmd_with_arg.cmd & MEMORY_TYPE)
		{
			UnsetCommandBitCode(&command->CPU_cmd_with_arg.cmd, MEMORY_TYPE);
			SetCommandTypeBitCode(&command->cmd_arg_type, MEMORY_TYPE);
		}

		if((char)command->CPU_cmd_with_arg.cmd & REGISTER_TYPE)
		{
			UnsetCommandBitCode(&command->CPU_cmd_with_arg.cmd, REGISTER_TYPE);
			SetCommandTypeBitCode(&command->cmd_arg_type, REGISTER_TYPE);

			if(!IsValidRegArg(command))
				return false;

			command->arguments_num = 1;
		}
		else if((char)command->CPU_cmd_with_arg.cmd & NUMBER_TYPE)
		{
			UnsetCommandBitCode(&command->CPU_cmd_with_arg.cmd, NUMBER_TYPE);
			SetCommandTypeBitCode(&command->cmd_arg_type, NUMBER_TYPE);
		
			if(!IsValidNumArg(command))
				return false;

			command->arguments_num = 1;
		}

	}
	else if(command->CPU_cmd_with_arg.arg != 0)
		command->error == INVALID_SYNTAX;
	else
		command->arguments_num = 0;


	switch (command->CPU_cmd_with_arg.cmd)
	{
		#define CMD_DEF(name, code, needed_args_num)			\
		case name:												\
			return (command->arguments_num == needed_args_num);
		
		#include "../../cpu_source/cmds_defs.h"

		default:
			return false;
	}
	#undef CMD_DEF
	return true;
}

unsigned CPUProcessFile(CPU* cpu)
{
	assert(cpu != NULL);

	ERROR_PROCESSING(cpu, CPUVerifier, CPUDump, CPUDtor, BEFORE_PROCRESSING_FILE);

	size_t* line_num = &cpu->current_line_num;
	Command* command = &cpu->current_command;

	for (*line_num = 1; *line_num < cpu->commands_num + 1; (*line_num)++)
	{

		command->CPU_cmd_with_arg.cmd  = *(CPUCommand*)((char*)cpu->CS + (*line_num-1) * sizeof(CPUCommandWithArg));
		command->CPU_cmd_with_arg.arg  = *(Elem_t*)(   (char*)cpu->CS + (*line_num-1) * sizeof(CPUCommandWithArg) + 8);

		if (IsValidCommand(command))
		{ 
			Elem_t num_to_output = {};
			Elem_t input_num = {};
			size_t correct_inputs_num = 0;

			switch (command->CPU_cmd_with_arg.cmd)
			{
				#define CMD_DEF(name, cpu_code, num_of_args, handle) case name: handle; break;
				
				#include "../../cpu_source/cmds_defs.h"

				#undef CMD_DEF
			}

		}
		else
			SetErrorBit(&cpu->errors, CPU_WRONG_INPUT);

		ERROR_PROCESSING(cpu, CPUVerifier, CPUDump, CPUDtor, *line_num);

		*command = {};
	}
}

Elem_t GetProperArgument(CPU* cpu)
{
	assert(cpu != NULL);

	CommandArgType cmd_type = cpu->current_command.cmd_arg_type;
	Elem_t cmd_arg  = cpu->current_command.CPU_cmd_with_arg.arg;

	if (cmd_type & NUMBER_TYPE && cmd_type & MEMORY_TYPE)
		return cpu->RAM[(size_t)cmd_arg];
	else if (cmd_type & REGISTER_TYPE & MEMORY_TYPE)		        
		return cpu->RAM[(size_t)GetReg(cpu, cmd_arg)];         
	else if (cmd_type & NUMBER_TYPE)			            
		return cmd_arg;
	else if (cmd_type & REGISTER_TYPE)											
		return GetReg(cpu, cmd_arg);            
	else
		assert(0);
}

void SetReg(CPU* cpu, Elem_t reg, Elem_t value)
{
	assert(cpu != NULL);

	switch ((int)reg)
	{
		#define REG_DEF(reg_name, reg_value)     \
		case reg_name:						     \
			cpu->reg_name = value;				 \
			break;							     \

		#include "../../cpu_source/regs_defs.h"
		
		#undef REG_DEF
	}
}

Elem_t GetReg(CPU* cpu, Elem_t reg)
{
	assert(cpu != NULL);

	switch ((int)reg)
	{
		#define REG_DEF(reg_name, ...)       \
		case reg_name:						 \
			return cpu->reg_name;		     \

		#include "../../cpu_source/regs_defs.h"

		#undef REG_DEF
	}
}

unsigned CPUCtor(CPU* cpu, const char* file_path)
{
	if (!cpu)
		return CPU_PTR_NULL;
	
	size_t compiled_file_size = GetFileSize(file_path);
	cpu->commands_num = compiled_file_size/sizeof(CPUCommandWithArg);
	
	fopen_s(&cpu->compiled_file, file_path, "r");
	if(!cpu->compiled_file)
	{
		SetErrorBit(&cpu->errors, CPU_COMPILED_FILE_ERROR);
		return cpu->errors;
	}

	fopen_s(&cpu->logger, "cpu_log.txt", "w");
	if (!cpu->logger)
		SetErrorBit(&cpu->errors, CPU_LOGER_ERROR);

	cpu->CS = (CPUCommandWithArg*)calloc(sizeof(char), compiled_file_size);
	if (!cpu->CS)
	{
		SetErrorBit(&cpu->errors, CPU_CS_PTR_NULL);
		return cpu->errors;
	}

	fread(cpu->CS, sizeof(char), compiled_file_size,cpu->compiled_file);

	if (StackCtor(&cpu->stack))
		SetErrorBit(&cpu->errors, CPU_BAD_STACK);

	ERROR_PROCESSING(cpu, CPUVerifier, CPUDump, CPUDtor, BEFORE_PROCRESSING_FILE);

	return 0;
}

unsigned CPUDtor(CPU* cpu)
{
	if (!cpu)
		return CPU_PTR_NULL;

	if (!(cpu->errors & CPU_DELETED))
	{
		fclose(cpu->compiled_file);
		fclose(cpu->logger);

		free(cpu->CS);

		unsigned destructor_errors = 0;
		if (StackDtor(&cpu->stack))
			SetErrorBit(&destructor_errors, CPU_BAD_STACK);

		return destructor_errors;
	}
	else
		return CPU_DELETED;
}