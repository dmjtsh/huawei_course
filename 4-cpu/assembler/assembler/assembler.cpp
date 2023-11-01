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
	assert(assembler != NULL);
	assert(logger != NULL);

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
		if (assembler->errors & INVALID_ASM_COMMAND)     fprintf(logger, "WRONG INPUT\n");
		if (assembler->errors & ASM_LOGER_ERROR)         fprintf(logger, "ASM LOGGER ERROR\n");
	    if (assembler->errors & ASM_COMPILED_FILE_ERROR) fprintf(logger, "COMPILED FILE ERROR\n");
		CommandDump(&assembler->current_command, logger);

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
	assert(assembler != NULL);

	if (!assembler)
		return ASM_PTR_NULL;

	CHECK_ERROR(assembler, assembler->logger == NULL,             ASM_LOGER_ERROR)
	CHECK_ERROR(assembler, assembler->compiled_file == NULL,      ASM_COMPILED_FILE_ERROR);
	CHECK_ERROR(assembler, !assembler->text_info.is_okay,         ASM_BAD_TEXT_INFO)
	CHECK_ERROR(assembler, assembler->current_command.error > 0,  ASM_CURRENT_COMMAND_ERROR)

	return assembler->errors;
}

bool CheckNumTypeAndSetArgCode(Command* command)
{
	assert(command != NULL);

	bool is_str_num = true;
	int num_arg = StrToNum(command->ASM_cmd_arg, command->asm_cmd_arg_len, &is_str_num);
	if(is_str_num)
	{
		if (command->CPU_cmd_code == POP)
			command->error = POP_WITH_NUM;
		command->CPU_cmd_arg = num_arg;
		return true;
	}
	return false;
}

bool CheckLabelTypeAndSetArgCode(ASM* assembler, Command* command)
{
	assert(assembler != NULL);
	assert(command != NULL);	
	
	char label_cmd_arg[MAX_ARG_LENGTH];
	strcpy(label_cmd_arg, command->ASM_cmd_arg);
	strcat(label_cmd_arg, ":");
	label_cmd_arg;

	for (size_t i = 0; i < assembler->labels_num; i++)
	{
		if(strcmp(label_cmd_arg, assembler->labels[i].label_name) == 0)
		{
			command->CPU_cmd_arg = assembler->labels[i].label_address;
			return true;
		}
	}
	return false;
}

bool CheckRegTypeAndSetArgCode(Command* command)
{
	assert(command != NULL);

	bool was_reg_found = false;
	size_t cpu_arg_code = 0;

	#define REG_DEF(name_of_reg, cpu_code_of_reg)                             \
	if(!was_reg_found && strcmp(#name_of_reg, command->ASM_cmd_arg) == 0)     \
	{                                                                         \
		was_reg_found = true;                                                 \
		cpu_arg_code = cpu_code_of_reg;                                       \
	}

	#include "../../regs_defs.h"

	#undef REG_DEF

	if(was_reg_found)
		command->CPU_cmd_arg = cpu_arg_code;

	return was_reg_found;
}             

bool CheckMemoryTypeAndSetArgCode(ASM* assembler, Command* command)
{
	assert(assembler != NULL);
	assert(command != NULL);

	if (command->ASM_cmd_arg[0] == '[' && command->ASM_cmd_arg[command->asm_cmd_arg_len-1] == ']')
	{
		char exp_in_brackets[MAX_ARG_LENGTH] = {};
		sscanf(command->ASM_cmd_arg, "[%[^]]", exp_in_brackets);
		exp_in_brackets;
		strcpy(command->ASM_cmd_arg, exp_in_brackets);
		command->ASM_cmd_arg;
		if (CheckNumTypeAndSetArgCode(command) || CheckLabelTypeAndSetArgCode(assembler, command) || CheckRegTypeAndSetArgCode(command))
			return true;
	}
	return false;
}

CmdArgType DefineTypeOfCmdArg(ASM* assembler, Command* command)
{
	assert(assembler != NULL);
	assert(command != NULL);
	
	if (CheckMemoryTypeAndSetArgCode(assembler, command))
		return MEMORY_TYPE;

	if (CheckNumTypeAndSetArgCode(command))
		return NUMBER_TYPE;

	if (CheckLabelTypeAndSetArgCode(assembler, command))
		return LABEL_TYPE;

	if (CheckRegTypeAndSetArgCode(command))
		return REGISTER_TYPE;

	return WRONG_TYPE;
}

bool CheckCmdArgsValid(ASM* assembler, Command* command, size_t needed_num_of_args) 
{
	assert(command != NULL);
	
	if (command->error > 0)
		return false;

	if (command->arguments_num > needed_num_of_args)
	{
		command->error = TOO_MANY_ARGS;
		return false;
	}
	else if (command->arguments_num < needed_num_of_args)
	{
		command->error = TOO_FEW_ARGS;
		return false;
	}

	if (command->arguments_num > 0)
	{
		CmdArgType type_of_cmd_arg = DefineTypeOfCmdArg(assembler, command);
		switch(type_of_cmd_arg)
		{
		case NUMBER_TYPE: case LABEL_TYPE:
			SetCmdBitCode(&command->CPU_cmd_code, NUMBER_TYPE);
			break;
		case REGISTER_TYPE:
			SetCmdBitCode(&command->CPU_cmd_code, REGISTER_TYPE);
			break;
		case MEMORY_TYPE:
			SetCmdBitCode(&command->CPU_cmd_code, MEMORY_TYPE);
			break;
		default:
			SetErrorBit(&assembler->errors, ASM_CURRENT_COMMAND_ERROR);
		}
	}
	return true;
}

bool IsCmdLabel(Command* command)
{
	assert(command != NULL);

	if (command->ASM_cmd_code[command->asm_cmd_len-1] == ':')
		return true;
	return false;
}

bool CheckValidOfCmdAndSetCPUCode(ASM* assembler, Command* command) 
{
	assert(assembler != NULL);
	assert(command != NULL);
	
	if (command->error > 0)
		return false;

	bool was_command_found = false;
	#define CMD_DEF(name, cpu_code, num_of_args, ...)                    \
	if (!was_command_found && strcmp(command->ASM_cmd_code, #name) == 0) \
	{                                                                    \
		was_command_found = true;                                        \
		command->CPU_cmd_code = (CPUCommand)cpu_code;                    \
		CheckCmdArgsValid(assembler, command, num_of_args);              \
	}

	#include "../../cmds_defs.h"

	#undef CMD_DEF

	if(!was_command_found && !IsCmdLabel)
		command->error = INVALID_SYNTAX;

	return was_command_found;
}

void ASMCompileCommand(ASM* assembler, Command* command, size_t num_of_command)
{
	assert(assembler != NULL);
	assert(command != NULL);

	assembler->CS[num_of_command-1].cpu_comand = command->CPU_cmd_code;
	if (command->arguments_num > 0)
		assembler->CS[num_of_command-1].arg    = command->CPU_cmd_arg;

	#ifdef _DEBUG
	if (command->arguments_num > 0)
		fprintf(assembler->debug_compiled_file, "%d %lf", command->CPU_cmd_code, command->CPU_cmd_arg);
	else
		fprintf(assembler->debug_compiled_file, "%d", command->CPU_cmd_code);

	fprintf(assembler->debug_compiled_file, "\n");
	#endif
}

void ASMProcessFile(ASM* assembler)
{
	assert(assembler != NULL);

	ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, BEFORE_PROCRESSING_FILE)

	#define COMMAND assembler->current_command

	// FIRST PASSAGE
	for (size_t num_of_line = 1; num_of_line < assembler->text_info.strings_num + 1; num_of_line++)
	{
		char* source_command_str = assembler->text_info.text_strings[num_of_line - 1].str;

		ASM_CMD_SSCANF(source_command_str, COMMAND);

		if (IsCmdLabel(&COMMAND))
		{
			if (COMMAND.arguments_num > 0)
				COMMAND.error = TOO_MANY_ARGS;

			strcpy(assembler->labels[assembler->labels_num].label_name, COMMAND.ASM_cmd_code);
			assembler->labels[assembler->labels_num].label_address = num_of_line - assembler->labels_num;
			assembler->labels_num++;
		}

		ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, num_of_line)
	}
	// SECOND PASSAGE
	for (size_t num_of_line = 1; num_of_line < assembler->text_info.strings_num + 1; num_of_line++)
	{
		char* source_command_str = assembler->text_info.text_strings[num_of_line - 1].str;

	/*size_t length_of_cmd = 0;																															 
	size_t length_of_cmd_w_arg = 0;																														 
	COMMAND.arguments_num = sscanf(source_command_str, "%s%n %s%n",  COMMAND.ASM_cmd_code, &length_of_cmd, COMMAND.ASM_cmd_arg, &length_of_cmd_w_arg) - 1;
	COMMAND.asm_cmd_len = length_of_cmd;																											 
	if (COMMAND.arguments_num == 0)																														 
		length_of_cmd_w_arg = length_of_cmd;
	else if(COMMAND.arguments_num > 0)																													 
		COMMAND.asm_cmd_arg_len = length_of_cmd_w_arg - length_of_cmd - 1;

	for (size_t i = length_of_cmd_w_arg; source_command_str[i] != '\0'; i++)                                                                             
	{																																					 																		 
		if(source_command_str[i] == ';')																												 
		{																																				 
			source_command_str[i] = '\0';																												 
			break;																																		 
		}																																				 
		if(source_command_str[i] != 32 && source_command_str[i] != '\0')																				 
		{																																				 
			SetErrorBit((unsigned*)&COMMAND.error, TOO_MANY_ARGS);																						 
			break;																																		 
		}																																				   
	}*/					

		ASM_CMD_SSCANF(source_command_str, COMMAND);
		assembler->current_command;
		if(CheckValidOfCmdAndSetCPUCode(assembler, &COMMAND))
			ASMCompileCommand(assembler, &COMMAND, num_of_line);

		ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, num_of_line)
	}
	#undef COMMAND

	fwrite(assembler->CS, sizeof(CpuCommandWithArg), assembler->text_info.strings_num, assembler->compiled_file);
}


int ASMCtor(ASM* assembler, const char* original_file_path, const char* compiled_file_path)
{
	if (!assembler)
		return ASM_PTR_NULL;

	fopen_s(&assembler->compiled_file, compiled_file_path, "wb");
	#ifdef _DEBUG
	fopen_s(&assembler->debug_compiled_file, "debug_compiled.txt", "wb");
	#endif
	fopen_s(&assembler->logger, "asm_log.txt", "wb");

	if (!assembler->compiled_file)
		SetErrorBit(&assembler->errors, ASM_COMPILED_FILE_ERROR);
	if (TextInfoCtor(&assembler->text_info, original_file_path))
		SetErrorBit(&assembler->errors, ASM_BAD_TEXT_INFO);

	assembler->labels_num = 0;
	for (size_t i = 0; i < LABELS_ARR_SIZE; i++)
		assembler->labels[i].label_address = -1;

	assembler->CS = (CpuCommandWithArg*)calloc(assembler->text_info.strings_num, sizeof(CpuCommandWithArg));

	ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, BEFORE_PROCRESSING_FILE)
	return assembler->errors;
}


int ASMDtor(ASM* assembler)
{
	if (!assembler)
		return ASM_PTR_NULL;

	fclose(assembler->compiled_file);

	#ifdef _DEBUG
	fclose(assembler->debug_compiled_file);
	#endif

	free(assembler->CS);

	unsigned destructor_errors = 0;
	if (TextInfoDtor(&assembler->text_info))
		SetErrorBit(&destructor_errors, ASM_BAD_TEXT_INFO);

	fclose(assembler->logger);

	return destructor_errors;
}