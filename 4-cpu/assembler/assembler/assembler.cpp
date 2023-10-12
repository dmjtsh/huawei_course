#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "assembler.h"
#include "../../error_processing.h"

void ASMDump(ASM* assembler, size_t num_of_line, FILE* logger)
{
	static size_t num_of_call = 1;
	fprintf(logger, 
	"=======================================\n"
		"ASM DUMP CALL #%zu\n"
		"Line: ", num_of_call);
	if (num_of_line == BEFORE_PROCRESSING_FILE)
		fprintf(logger, "Before processing file\n");
	else
		fprintf(logger, "%zu\n", num_of_line);

	if (assembler->errors)
	{
		fprintf(logger,
		"-------------ERRORS------------\n");
		if (assembler->errors & ASM_PTR_NULL)
		{
			fprintf(logger, "ASM POINTER IS NULL\n");
			return;
		}
		if (assembler->errors & ASM_BAD_TEXT_INFO)		 fprintf(logger, "SOME TROUBLES WITH TEXT INFO STRUCT\n");
		if (assembler->errors & ASM_WRONG_INPUT)         fprintf(logger, "WRONG INPUT\n");
		if (assembler->errors & ASM_WRONG_COMMAND_USAGE) fprintf(logger, "WRONG COMMAND USAGE\n");
		if (assembler->errors & ASM_LOGER_ERROR)         fprintf(logger, "ASM LOGGER ERROR\n");
	    if (assembler->errors & ASM_COMPILED_FILE_ERROR) fprintf(logger, "COMPILED FILE ERROR\n"); 

		fprintf(logger,
		"----------END_OF_ERRORS--------\n");
	}
	else
		fprintf(logger,
		"------------NO_ERRORS----------\n"
	"=======================================\n\n");

}

int ASMVerifier(ASM* assembler)
{
	if (!assembler)
		return ASM_PTR_NULL;

	CHECK_ERROR(assembler, assembler->logger == NULL,        ASM_LOGER_ERROR)
	CHECK_ERROR(assembler, assembler->compiled_file == NULL, ASM_COMPILED_FILE_ERROR);
	CHECK_ERROR(assembler, !assembler->text_info.is_okay,    ASM_BAD_TEXT_INFO)

	return assembler->errors;
}

void SetCPUCodeOfCommand(Command* command)
{	
	command->CPU_cmd_code = INVALID_CPU_COMMAND;

	#define CMD_DEF(name, cpu_code, num_of_args, ...)      \
	 if (strcmp(command->ASM_cmd_code, #name) == 0)        \
	 {                                                     \
		 if(command->arguments_num == num_of_args)         \
			 command->CPU_cmd_code = (CPUCommand)cpu_code; \
	 }                                                     \

	CMD_DEF(PUSH, 2, 1,
	StackPush(&cpu->stack, command.cmd_arg);)

	#include "../../cmds_defs.h"
	#undef CMD_DEF
}

void ASMCompileCommand(ASM* assembler, Command* command)
{
	if (command->arguments_num == 0)
		COMMAND_NO_ARG_FPRINTF(assembler->compiled_file, (Elem_t)command->CPU_cmd_code)
	else
		COMMAND_FPRINTF(assembler->compiled_file, (Elem_t)command->CPU_cmd_code, command->cmd_arg)
	fprintf(assembler->compiled_file, "\n");
}



void ASMProcessFile(ASM* assembler)
{
	size_t asm_errors = 0;
	ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, BEFORE_PROCRESSING_FILE)

	Command command = {};

	for (size_t num_of_line = 1; num_of_line < assembler->text_info.strings_num + 1; num_of_line++)
	{
		command.arguments_num = ELEM_T_ASM_SSCANF(assembler->text_info.text_strings[num_of_line - 1].str, command.ASM_cmd_code, &command.cmd_arg) - 1;
		
	    SetCPUCodeOfCommand(&command);
		if (command.CPU_cmd_code != INVALID_CPU_COMMAND)
			ASMCompileCommand(assembler, &command);
		else
			SetErrorBit(&assembler->errors, ASM_WRONG_INPUT);

		ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, num_of_line)
	}
}


int ASMCtor(ASM* assembler, const char* original_file_path, const char* compiled_file_path)
{
	if (!assembler)
		return ASM_PTR_NULL;

	fopen_s(&assembler->compiled_file, compiled_file_path, "wb");

	fopen_s(&assembler->logger, "asm_log.txt", "wb");

	if (!assembler->compiled_file)
		SetErrorBit(&assembler->errors, ASM_COMPILED_FILE_ERROR);

	if (TextInfoCtor(&assembler->text_info, original_file_path))
		SetErrorBit(&assembler->errors, ASM_BAD_TEXT_INFO);

	ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, BEFORE_PROCRESSING_FILE)
	return assembler->errors;
}


int ASMDtor(ASM* assembler)
{
	if (!assembler)
		return ASM_PTR_NULL;

	fclose(assembler->compiled_file);

	fclose(assembler->logger);

	unsigned destructor_errors = 0;
	if (TextInfoDtor(&assembler->text_info))
		SetErrorBit(&destructor_errors, ASM_BAD_TEXT_INFO);

	return destructor_errors;
}