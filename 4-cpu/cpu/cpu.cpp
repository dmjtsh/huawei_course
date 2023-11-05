#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "cpu.h"
#include "../../commands.h"
#include "../../DimasLIB/DimasUtilities/utilities.h"
#include "../../error_processing.h"

void CPUDump(CPU* cpu, size_t num_of_line, FILE* logger)
{
	static size_t num_of_call = 1;
	fprintf(logger, 
	"=======================================\n"
		"CPU DUMP CALL #%zu\n"
		"Line: ", num_of_call);
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
			if (current_line == cpu->current_line_num+1)
				fprintf(logger, " <-----");
			fprintf(logger, "\n");
		}
	}
	fprintf(logger, "\nRAX:" ELEM_T_IDENTIFIER ", RBX:" ELEM_T_IDENTIFIER ", RCX:" ELEM_T_IDENTIFIER ", RDX:" ELEM_T_IDENTIFIER "\n", 
			cpu->RAX, cpu->RBX, cpu->RCX, cpu->RDX);
	fprintf(logger,
	"=======================================\n\n");

}

int CPUVerifier(CPU* cpu)
{
	if (!cpu)
		return CPU_PTR_NULL;

	CHECK_ERROR(cpu, cpu->stack.errors,   CPU_BAD_STACK)
	CHECK_ERROR(cpu, cpu->logger == NULL, CPU_LOGER_ERROR)
	CHECK_ERROR(cpu, cpu->errors,         CPU_CURRENT_COMMAND_ERROR)

	return cpu->errors;
}

#define CPU_ERROR_PROCESSING(cpu, line_of_file) \
	cpu_errors = CPUVerifier(cpu);              \
	CPUDump(cpu, line_of_file, cpu->logger);    \
	if (cpu_errors)                             \
	{                                           \
		CPUDump(cpu, line_of_file, stderr);     \
		CPUDtor(cpu);                           \
		abort();                                \
	}                                           \

bool IsValidNumArg(Command* command)
{
	assert(command != NULL);

	if(command->CPU_cmd_code == POP)
	{
		command->error = POP_WITH_NUM;
		return false;
	}
	return true;
}

bool IsValidRegArg(Command* command)
{
	assert(command != NULL);

	bool was_reg_found = false;
	#define REG_DEF(name, cpu_code, ...)          \
	if (cpu_code == (size_t)command->CPU_cmd_arg) \
		was_reg_found = true;                     \
	
	#include "../../regs_defs.h"
	#undef REG_DEF
	
	if(!was_reg_found)
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

	if ((short)command->CPU_cmd_code & (MEMORY_TYPE | NUMBER_TYPE | REGISTER_TYPE) && command->CPU_cmd_code != HLT)
	{
		command->arguments_num = 1;
		if((short)command->CPU_cmd_code & MEMORY_TYPE)
		{
			UnsetCommandBitCode(&command->CPU_cmd_code, MEMORY_TYPE);
			SetCommandTypeBitCode(&command->cmd_arg_type, MEMORY_TYPE);
		}

		if((short)command->CPU_cmd_code & REGISTER_TYPE)
		{
			UnsetCommandBitCode(&command->CPU_cmd_code, REGISTER_TYPE);
			SetCommandTypeBitCode(&command->cmd_arg_type, REGISTER_TYPE);

			if(!IsValidRegArg(command))
				return false;

			command->arguments_num = 1;
		}
		else if((short)command->CPU_cmd_code & NUMBER_TYPE)
		{
			UnsetCommandBitCode(&command->CPU_cmd_code, NUMBER_TYPE);
			SetCommandTypeBitCode(&command->cmd_arg_type, NUMBER_TYPE);
		
			if(!IsValidNumArg(command))
				return false;

			command->arguments_num = 1;
		}

	}
	else if(command->CPU_cmd_arg != 0)
		command->error == INVALID_SYNTAX;
	else
		command->arguments_num = 0;


	switch (command->CPU_cmd_code)
	{
		#define CMD_DEF(name, code, needed_args_num)       \
		case name:				                           \
			if (command->arguments_num == needed_args_num) \
				return true;		                       \
			else                                           \
				return false;                              \
		
		#include "../../cmds_defs.h"

		default:
			return false;
	}
	#undef CMD_DEF
	return true;
}

void CPUProcessFile(CPU* cpu)
{
	assert(cpu != NULL);

	size_t cpu_errors = 0;
	CPU_ERROR_PROCESSING(cpu, BEFORE_PROCRESSING_FILE);

	size_t* line_num = &cpu->current_line_num;
	Command* command = &cpu->current_command;

	for (*line_num = 1; *line_num < cpu->commands_num + 1; (*line_num)++)
	{
		command->CPU_cmd_code = *(CPUCommand*)((char*)cpu->CS + (*line_num-1) * sizeof(CPUCommandWithArg));
		command->CPU_cmd_arg  = *(Elem_t*)(    (char*)cpu->CS + (*line_num-1) * sizeof(CPUCommandWithArg) + 8);
		
		if (*line_num == 4)
			printf("\n");

		if (IsValidCommand(command))
		{ 
			Elem_t num_to_output = {};
			Elem_t input_num = {};
			size_t correct_inputs_num = 0;
			switch (command->CPU_cmd_code)
			{
				#define CMD_DEF(name, cpu_code, num_of_args, handle) case name: handle; break;
				
				#include "../../cmds_defs.h"

				#undef CMD_DEF
			}

		}
		else
			SetErrorBit(&cpu->errors, CPU_WRONG_INPUT);

		CPU_ERROR_PROCESSING(cpu, *line_num);

		*command = {};
	}

	#undef CURRENT_LINE_NUM
	#undef CURRENT_COMMAND
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

		#include "C:\Users\79370\source\repos\regs_defs.h"
		
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

		#include "C:\Users\79370\source\repos\regs_defs.h"

		#undef REG_DEF
	}
}

int CPUCtor(CPU* cpu, const char* file_path)
{
	if (!cpu)
		return CPU_PTR_NULL;
	
	size_t compiled_file_size = GetFileSize(file_path);
	cpu->commands_num = compiled_file_size/sizeof(CPUCommandWithArg);
	
	fopen_s(&cpu->compiled_file, file_path, "r");
	assert(cpu->compiled_file != NULL);

	cpu->CS = (CPUCommandWithArg*)calloc(sizeof(char), compiled_file_size);
	assert(cpu->CS != NULL);
	fread(cpu->CS, sizeof(char), compiled_file_size,cpu->compiled_file);

	if (StackCtor(&cpu->stack))
		SetErrorBit(&cpu->errors, CPU_BAD_STACK);

	fopen_s(&cpu->logger, "cpu_log.txt", "w");
	if (!cpu->logger)
		SetErrorBit(&cpu->errors, CPU_LOGER_ERROR);

	size_t cpu_errors = 0;
	CPU_ERROR_PROCESSING(cpu, BEFORE_PROCRESSING_FILE);

	return 0;
}

int CPUDtor(CPU* cpu)
{
	if (!cpu)
		return CPU_PTR_NULL;

	fclose(cpu->compiled_file);
	free(cpu->CS);

	fclose(cpu->logger);

	unsigned destructor_errors = 0;
	if (StackDtor(&cpu->stack))
		SetErrorBit(&destructor_errors, CPU_BAD_STACK);

	return cpu->errors;
}