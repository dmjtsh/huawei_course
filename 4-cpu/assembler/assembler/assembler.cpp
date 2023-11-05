#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "assembler.h"
#include "../../error_processing.h"

void ASMDump(ASM* assembler, size_t line_num, FILE* logger)
{
	assert(assembler != NULL);
	assert(logger != NULL);

	static size_t num_of_call = 1;
	fprintf(logger, 
	"=======================================\n"
		"ASM DUMP CALL #%zu\n"
		"Line: ", num_of_call);
	if (line_num == BEFORE_PROCRESSING_FILE)
		fprintf(logger, "Before processing file\n");
	else
		fprintf(logger, "%zu\n", line_num);

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

bool TrySetNumArg(Command* command)
{
	assert(command != NULL);

	bool is_str_num = true;

	size_t arg_len = strlen(command->ASM_cmd_arg);
	Elem_t num_arg = StrToNum(command->ASM_cmd_arg, arg_len, &is_str_num);
	if(is_str_num)
	{
		if(command->CPU_cmd_code == POP)
			command->error = POP_WITH_NUM;
		command->CPU_cmd_arg = num_arg;
		return true;
	}
	return false;
}

bool TrySetLabelArg(ASM* assembler, Command* command)
{
	assert(assembler != NULL);
	assert(command != NULL);	
	
	char label_cmd_arg[MAX_ARG_LENGTH];
	strcpy(label_cmd_arg, command->ASM_cmd_arg);
	strcat(label_cmd_arg, ":");

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

bool TrySetRegArg(Command* command)
{
	assert(command != NULL);

	bool was_reg_found = false;
	size_t cpu_arg_code = 0;

	#define REG_DEF(reg_name, reg_cpu_code)                                \
	if(!was_reg_found && strcmp(#reg_name, command->ASM_cmd_arg) == 0)     \
	{                                                                      \
		was_reg_found = true;                                              \
		cpu_arg_code = reg_cpu_code;                                       \
	}

	#include "../../regs_defs.h"

	#undef REG_DEF

	if(was_reg_found)
		command->CPU_cmd_arg = cpu_arg_code;

	return was_reg_found;
}             

bool TrySetMemoryArg(ASM* assembler, Command* command)
{
	assert(assembler != NULL);
	assert(command != NULL);

	size_t arg_len = strlen(command->ASM_cmd_arg);
	if (command->ASM_cmd_arg[0] == '[' && command->ASM_cmd_arg[arg_len-1] == ']')
	{
		char exp_in_brackets[MAX_ARG_LENGTH] = {};
		sscanf(command->ASM_cmd_arg, "[%[^]]", exp_in_brackets);
		strcpy(command->ASM_cmd_arg, exp_in_brackets);
		return true;
	}

	return false;
}

void ResolveTypeOfCommandArg(ASM* assembler, Command* command)
{
	assert(assembler != NULL);
	assert(command != NULL);

	if(TrySetMemoryArg(assembler, command))
		SetCommandBitCode(&command->CPU_cmd_code, MEMORY_TYPE);

	if (TrySetNumArg(command))
	{
		SetCommandBitCode(&command->CPU_cmd_code, NUMBER_TYPE);
		return;
	}

	if (TrySetLabelArg(assembler, command))
	{
		SetCommandBitCode(&command->CPU_cmd_code, NUMBER_TYPE);
		return;
	}

	if (TrySetRegArg(command))
	{
		SetCommandBitCode(&command->CPU_cmd_code, REGISTER_TYPE);
		return;
	}

	command->error = INVALID_REG_OR_LABEL_NAME;
}

bool TrySetArgCPUCode(ASM* assembler, Command* command, size_t needed_args_num) 
{
	assert(assembler != NULL);
	assert(command != NULL);
	
	if (command->error > 0)
		return false;

	if (command->arguments_num > needed_args_num)
	{
		command->error = TOO_MANY_ARGS;
		return false;
	}
	else if (command->arguments_num < needed_args_num)
	{
		command->error = TOO_FEW_ARGS;
		return false;
	}

	if (command->arguments_num > 0)
		ResolveTypeOfCommandArg(assembler, command);

	return true;
}

bool IsCommandLabel(Command* command)
{
	assert(command != NULL);

	size_t command_len = strlen(command->ASM_cmd_code);
	return command->ASM_cmd_code[command_len - 1] == ':';
}

bool TrySetCommandCPUCode(ASM* assembler, Command* command) 
{
	assert(assembler != NULL);
	assert(command != NULL);
	
	if (command->error > 0)
		return false;

	bool is_command_args_valid = false;
	bool was_command_found	   = false;

	#define CMD_DEF(name, cpu_code, args_num, ...)                       \
	if (!was_command_found && strcmp(command->ASM_cmd_code, #name) == 0) \
	{                                                                    \
		was_command_found = true;                                        \
		command->CPU_cmd_code = (CPUCommand)cpu_code;                    \
		if(TrySetArgCPUCode(assembler, command, args_num))               \
			is_command_args_valid = true;                                \
	}

	#include "../../cmds_defs.h"

	#undef CMD_DEF

	bool is_cmd_label = IsCommandLabel(command);
	if(!was_command_found && !is_cmd_label)
		command->error = INVALID_SYNTAX;
	else if(!is_command_args_valid && !is_cmd_label)
		command->error = INVALID_REG_OR_LABEL_NAME;

	return was_command_found;
}

void WriteCommandToFile(ASM* assembler, Command* command)
{
	assert(assembler != NULL);
	assert(command != NULL);

	static size_t write_cmd_index = 0;

	assembler->CS[write_cmd_index].cpu_comand = command->CPU_cmd_code;
	if (command->arguments_num > 0)
		assembler->CS[write_cmd_index].arg    = command->CPU_cmd_arg;

	#ifdef _DEBUG
	if (command->arguments_num > 0)
		fprintf(assembler->debug_compiled_file, "%d %lf", command->CPU_cmd_code, command->CPU_cmd_arg);
	else
		fprintf(assembler->debug_compiled_file, "%d", command->CPU_cmd_code);

	fprintf(assembler->debug_compiled_file, "\n");
	#endif

	write_cmd_index++;
}

const int SPACE = 32;
int SkipNonSpaces(char* source_command_str, size_t str_begin)
{
	assert(source_command_str != NULL);

	size_t char_num = str_begin;
	for(;source_command_str[char_num] != SPACE && source_command_str[char_num] != '\0' && source_command_str[char_num] != ';'; char_num++) { ; }

	return char_num;
}

int SkipSpaces(char* source_command_str, size_t str_begin)
{
	assert(source_command_str != NULL);
	
	size_t char_num = str_begin; 
	for (;source_command_str[char_num] == SPACE && source_command_str[char_num] != '\0'; char_num++) { ; }
	
	return char_num;
}

void ReadLine(char* source_command_str, Command* command)
{	
	assert(source_command_str != NULL);
	assert(command != NULL);

	size_t num_args = 0;
	int begin = SkipSpaces(source_command_str, 0);
	int end   = SkipNonSpaces(source_command_str, begin);
	strncpy(command->ASM_cmd_code, source_command_str + begin, end - begin);
	if(source_command_str[end] == ';')
		return;

	size_t arguments_num = 0;
	while (source_command_str[end] != '\0')
	{
		begin = SkipSpaces(source_command_str, end);
		end   = SkipNonSpaces(source_command_str, begin);
		strncat(command->ASM_cmd_arg, source_command_str + begin, end - begin);
		if(source_command_str[end] == ';')
			return;

		if (source_command_str[begin] == '[' && source_command_str[end-1] == ']')
			command->arguments_num++;
		else if ((end - begin > 1) || (source_command_str[begin] != '[' && source_command_str[end-1] != ']'))
			command->arguments_num++;
	}
}

void ASMProcessFile(ASM* assembler)
{
	assert(assembler != NULL);

	ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, BEFORE_PROCRESSING_FILE)

	Command* command = &assembler->current_command;
	size_t blank_lines_counter = 0;
	// FIRST PASSAGE
	for (size_t line_num = 1; line_num < assembler->text_info.strings_num + 1; line_num++)
	{
		char* source_command_str = assembler->text_info.text_strings[line_num - 1].str;
		
		ReadLine(source_command_str, command);
		if (*command->ASM_cmd_code == 0 && *command->ASM_cmd_arg == 0)
		{
			blank_lines_counter++;
			continue;
		}

		if (IsCommandLabel(command))
		{
			if (command->arguments_num > 0)
				command->error = TOO_MANY_ARGS;

			strcpy(assembler->labels[assembler->labels_num].label_name, command->ASM_cmd_code);
			assembler->labels[assembler->labels_num].label_address = line_num - assembler->labels_num - blank_lines_counter;
			assembler->labels_num++;
		}

		ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, line_num)

		*command = {};
	}

	// SECOND PASSAGE
	for (size_t line_num = 1; line_num < assembler->text_info.strings_num + 1; line_num++)
	{
		char* source_command_str = assembler->text_info.text_strings[line_num - 1].str;	

		ReadLine(source_command_str, command);

		if (*command->ASM_cmd_code == 0 && *command->ASM_cmd_arg == 0)
			continue;

		if(TrySetCommandCPUCode(assembler, command))
			WriteCommandToFile(assembler, command);
			
		ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, line_num)

		*command = {};
	}
	#undef COMMAND

	fwrite(assembler->CS, sizeof(CPUCommandWithArg), assembler->text_info.strings_num, assembler->compiled_file);
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

	if (!assembler->compiled_file || !assembler->debug_compiled_file)
		SetErrorBit(&assembler->errors, ASM_COMPILED_FILE_ERROR);
	if (!assembler->logger)
		SetErrorBit(&assembler->errors, ASM_LOGER_ERROR);
	if (TextInfoCtor(&assembler->text_info, original_file_path))
		SetErrorBit(&assembler->errors, ASM_BAD_TEXT_INFO);

	assembler->labels_num = 0;
	for (size_t i = 0; i < LABELS_ARR_SIZE; i++)
		assembler->labels[i].label_address = -1;

	assembler->CS = (CPUCommandWithArg*)calloc(assembler->text_info.strings_num, sizeof(CPUCommandWithArg));

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