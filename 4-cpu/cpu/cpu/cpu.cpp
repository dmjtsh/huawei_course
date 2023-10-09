#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "cpu.h"
#include "../../commands.h"
#include "../../DimasLIB/DimasUtilities/utilities.h"

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
		if (cpu->errors & CPU_BAD_STACK)				   fprintf(logger, "SOME TROUBLES WITH STACK STRUCT\n");
		if (cpu->errors & CPU_BAD_TEXT_INFO)			   fprintf(logger, "SOME TROUBLES WITH TEXT INFO STRUCT\n");
		if (cpu->errors & CPU_WRONG_INPUT)                 fprintf(logger, "WRONG INPUT\n");
		if (cpu->errors & CPU_WRONG_COMMAND_USAGE)         fprintf(logger, "WRONG COMMAND USAGE\n");

		fprintf(logger,
		"----------END_OF_ERRORS--------\n");
	}
	else
		fprintf(logger,
		"------------NO_ERRORS----------\n"
	"=======================================\n\n");

}

#define CPU_CHECK_ERROR(cpu, condition, error)	    \
	if(condition)									\
		SetErrorBit(&cpu->errors, error);			\
	else											\
		UnsetErrorBit(&cpu->errors, error);

int CPUVerifier(CPU* cpu)
{
	if (!cpu)
		return CPU_PTR_NULL;

	CPU_CHECK_ERROR(cpu, cpu->stack.errors,       CPU_BAD_STACK)
	CPU_CHECK_ERROR(cpu, !cpu->text_info.is_okay, CPU_BAD_TEXT_INFO)

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

void CPUProcessFile(CPU* cpu)
{
	size_t cpu_errors = 0;
	CPU_ERROR_PROCESSING(cpu, BEFORE_PROCRESSING_FILE);

	size_t num_of_line = 0;
	Command command = {};
	char* command_str = NULL;

	for (;num_of_line < cpu->text_info.strings_num; num_of_line++)
	{
		command_str = cpu->text_info.text_strings[num_of_line].str;
		command.arguments_num = ELEM_T_CPU_SSCANF(command_str, &command.CPU_cmd_code, &command.cmd_arg) - 1;
		if (IsValidCommand(&command))
		{ 
			Elem_t num_to_output = {};
			Elem_t input_num = {};
			size_t correct_inputs_num = 0;
			switch (command.CPU_cmd_code)
			{
			#define CMD_DEF(name, cpu_code, num_of_args, handle) case name: handle; break;
			
			#include "../../cmds_defs.h"

			#undef CMD_DEF
			}
		}
		else
			SetErrorBit(&cpu->errors, CPU_WRONG_INPUT);
		
		CPU_ERROR_PROCESSING(cpu, num_of_line);
	}
}

int CPUCtor(CPU* cpu, const char* file_path)
{
	if (!cpu)
		return CPU_PTR_NULL;

	fopen_s(&cpu->logger, "cpu_log.txt", "w");

	if (TextInfoCtor(&cpu->text_info, file_path))
		SetErrorBit(&cpu->errors, CPU_BAD_TEXT_INFO);

	if (StackCtor(&cpu->stack))
		SetErrorBit(&cpu->errors, CPU_BAD_STACK);

	size_t cpu_errors = 0;
	CPU_ERROR_PROCESSING(cpu, BEFORE_PROCRESSING_FILE);

	return 0;
}

int CPUDtor(CPU* cpu)
{
	if (!cpu)
		return CPU_PTR_NULL;

	if (TextInfoDtor(&cpu->text_info))
		SetErrorBit(&cpu->errors, CPU_BAD_TEXT_INFO);
	if (StackDtor(&cpu->stack))
		SetErrorBit(&cpu->errors, CPU_BAD_STACK);

	return cpu->errors;
}