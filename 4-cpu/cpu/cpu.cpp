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
			if (current_line == cpu->current_line_num)
				fprintf(logger, " <-----");
			fprintf(logger, "\n");
		}
	}
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

bool IsValidCommandNumArg(Command* command)
{
	assert(command != NULL);

	if(command->CPU_cmd_code == POP)
	{
		SetErrorBit((unsigned*)&command->error, POP_WITH_NUM);
		return false;
	}
	return true;
}

bool IsValidCommandRegArg(Command* command)
{
	assert(command != NULL);

	bool was_reg_found = false;
	#define REG_DEF(name, cpu_code, ...)   \
	if (cpu_code == command->CPU_cmd_code) \
		was_reg_found = true;              \
	
	#include "../../regs_defs.h"
	#undef REG_DEF
	
	if(!was_reg_found)
	{
		SetErrorBit((unsigned*)command->error, INVALID_REG_OR_LABEL_NAME);
		return false;
	}

	return true;
}

#define IS_VALID_CMD_ARG(cond) \
	if(!cond)                  \
		return false;          \

bool IsValidCommand(Command* command)
{
	assert(command != NULL);

	if (!command)
		return false;

	if(command->arguments_num >= 1)
	{
		if((short)command->CPU_cmd_code & REGISTER_TYPE)
		{
			command->cmd_arg_type = REGISTER_TYPE;
			UnsetCommandBitCode(&command->CPU_cmd_code, REGISTER_TYPE);
			IS_VALID_CMD_ARG(IsValidCommandRegArg(command))
		}
		else if((short)command->CPU_cmd_code & NUMBER_TYPE)
		{
			command->cmd_arg_type = NUMBER_TYPE;
			UnsetCommandBitCode(&command->CPU_cmd_code, NUMBER_TYPE);
			IS_VALID_CMD_ARG(IsValidCommandNumArg(command));

		}
		else if((short)command->CPU_cmd_code & MEMORY_NUM_TYPE)
		{
			command->cmd_arg_type = MEMORY_NUM_TYPE;
			UnsetCommandBitCode(&command->CPU_cmd_code, MEMORY_NUM_TYPE);
		}
		else if((short)command->CPU_cmd_code & MEMORY_REG_TYPE)
		{
			command->cmd_arg_type = MEMORY_REG_TYPE;
			UnsetCommandBitCode(&command->CPU_cmd_code, MEMORY_REG_TYPE);
			IsValidCommandRegArg(command);
		}
	}

	switch (command->CPU_cmd_code)
	{
		#define CMD_DEF(name, ...) \
		case name:				   \
			return true;           \
		
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

	#define CURRENT_LINE_NUM  cpu->current_line_num
	#define CURRENT_COMMAND   cpu->current_command

	for (CURRENT_LINE_NUM = 1; CURRENT_LINE_NUM <= cpu->commands_num; CURRENT_LINE_NUM++)
	{
		CURRENT_COMMAND.CPU_cmd_code = *(CPUCommand*)((char*)cpu->CS + (CURRENT_LINE_NUM-1) * sizeof(CPUCommandWithArg));
		CURRENT_COMMAND.CPU_cmd_arg  = *(Elem_t*)(    (char*)cpu->CS + (CURRENT_LINE_NUM-1) * sizeof(CPUCommandWithArg) + 8);
		if (CURRENT_COMMAND.CPU_cmd_code == 0 && CURRENT_COMMAND.CPU_cmd_arg == 0)
			continue;
		else if(CURRENT_COMMAND.CPU_cmd_arg == 0)
			CURRENT_COMMAND.arguments_num = 0;
		else
			CURRENT_COMMAND.arguments_num = 1;
		if (IsValidCommand(&CURRENT_COMMAND))
		{ 
			Elem_t num_to_output = {};
			Elem_t input_num = {};
			size_t correct_inputs_num = 0;
			switch (CURRENT_COMMAND.CPU_cmd_code)
			{
				#define CMD_DEF(name, cpu_code, num_of_args, handle) case name: handle; break;
				
				#include "../../cmds_defs.h"

				#undef CMD_DEF
			}
		}
		else
			SetErrorBit(&cpu->errors, CPU_WRONG_INPUT);
		CPU_ERROR_PROCESSING(cpu, CURRENT_LINE_NUM);
	}

	#undef CURRENT_LINE_NUM
	#undef CURRENT_COMMAND
}

#define REG_DEF(reg_name, reg_value)     \
case reg_name:						     \
	cpu->reg_name = value;				 \
	break;							     \

void SetReg(CPU* cpu, Elem_t reg, Elem_t value)
{
	assert(cpu != NULL);

	switch ((int)reg)
	{
		#include "C:\Users\79370\source\repos\regs_defs.h"
	}
}

#undef REG_DEF

#define REG_DEF(reg_name, ...)       \
case reg_name:						 \
	return cpu->reg_name;		     \

Elem_t GetReg(CPU* cpu, Elem_t reg)
{
	assert(cpu != NULL);

	switch ((int)reg)
	{
		#include "C:\Users\79370\source\repos\regs_defs.h"
	}
}

#undef REG_DEF

int CPUCtor(CPU* cpu, const char* file_path)
{
	if (!cpu)
		return CPU_PTR_NULL;

	fopen_s(&cpu->logger, "cpu_log.txt", "w");

	assert(cpu->logger != NULL);
	
	size_t compiled_file_size = GetFileSize(file_path);
	cpu->commands_num = compiled_file_size/sizeof(CPUCommandWithArg);
	fopen_s(&cpu->compiled_file, file_path, "r");
	
	assert(cpu->compiled_file != NULL);

	cpu->CS = (CPUCommandWithArg*)calloc(sizeof(char), compiled_file_size);
	fread(cpu->CS, sizeof(char), compiled_file_size,cpu->compiled_file);

	if (StackCtor(&cpu->stack))
		SetErrorBit(&cpu->errors, CPU_BAD_STACK);
	if (StackCtor(&cpu->func_stack))
		SetErrorBit(&cpu->errors, CPU_BAD_FUNC_STACK);

	size_t cpu_errors = 0;
	CPU_ERROR_PROCESSING(cpu, BEFORE_PROCRESSING_FILE);

	return 0;
}

int CPUDtor(CPU* cpu)
{
	if (!cpu)
		return CPU_PTR_NULL;

	fclose(cpu->logger);

	free(cpu->CS);
	fclose(cpu->compiled_file);

	unsigned destructor_errors = 0;
	if (StackDtor(&cpu->stack))
		SetErrorBit(&destructor_errors, CPU_BAD_STACK);
	if (StackDtor(&cpu->func_stack))
		SetErrorBit(&destructor_errors, CPU_BAD_STACK);

	return cpu->errors;
}