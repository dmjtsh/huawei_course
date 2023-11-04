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

bool CheckNumTypeAndSetArgCode(Command* command)
{
	assert(command != NULL);

	bool is_str_num = true;
	int num_arg = StrToNum(command->ASM_cmd_arg, command->ASM_cmd_arg_len, &is_str_num);
	if(is_str_num)
	{
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

struct CommandArg
{
	int arg_type = 0;
	union
	{
		int imm;
		int reg_num;
		int label;
		int ram;
	};
};

// TrySetCommandLabel( command_arg);
// TrySetCommandImm( command_arg);


bool CheckRegTypeAndSetArgCode(Command* command)
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

bool CheckMemoryTypeAndSetArgCode(ASM* assembler, Command* command, CommandArgType* memory_arg_type)
{
	assert(assembler != NULL);
	assert(command != NULL);

	if (command->ASM_cmd_arg[0] == '[' && command->ASM_cmd_arg[command->ASM_cmd_arg_len-1] == ']')
	{
		char exp_in_brackets[MAX_ARG_LENGTH] = {};
		sscanf(command->ASM_cmd_arg, "[%[^]]", exp_in_brackets);
		strcpy(command->ASM_cmd_arg, exp_in_brackets);
		command->ASM_cmd_arg_len -= 2;
		if (CheckNumTypeAndSetArgCode(command) || CheckLabelTypeAndSetArgCode(assembler, command))
		{
			*memory_arg_type = MEMORY_NUM_TYPE;
			return true;
		}
		else if(CheckRegTypeAndSetArgCode(command))
		{
			*memory_arg_type = MEMORY_REG_TYPE;
			return true;
		}
	}

	return false;
}

CommandArgType DefineTypeOfCmdArg(ASM* assembler, Command* command)
{
	assert(assembler != NULL);
	assert(command != NULL);
	
	CommandArgType memory_arg_type = WRONG_TYPE;
	if (CheckMemoryTypeAndSetArgCode(assembler, command, &memory_arg_type))
		return memory_arg_type;

	if (CheckNumTypeAndSetArgCode(command))
		return NUMBER_TYPE;

	if (CheckLabelTypeAndSetArgCode(assembler, command))
		return LABEL_TYPE;

	if (CheckRegTypeAndSetArgCode(command))
		return REGISTER_TYPE;

	return WRONG_TYPE;
}

bool CheckCmdArgsValid(ASM* assembler, Command* command, size_t needed_args_num) 
{
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
	{
		CommandArgType cmd_arg_type = DefineTypeOfCmdArg(assembler, command);
		switch(cmd_arg_type)
		{
		case NUMBER_TYPE: 
		case LABEL_TYPE:
			if (command->CPU_cmd_code == POP)
				command->error = POP_WITH_NUM;
			SetCommandBitCode(&command->CPU_cmd_code, NUMBER_TYPE);
			break;
		case REGISTER_TYPE:
			SetCommandBitCode(&command->CPU_cmd_code, REGISTER_TYPE);
			break;
		case MEMORY_NUM_TYPE:
			SetCommandBitCode(&command->CPU_cmd_code, MEMORY_NUM_TYPE);
			break;
		case MEMORY_REG_TYPE:
			SetCommandBitCode(&command->CPU_cmd_code, MEMORY_REG_TYPE);
			break;
		default:
			SetErrorBit(&assembler->errors, ASM_CURRENT_COMMAND_ERROR);
			return false;
		}
	}
	return true;
}

bool IsCommandLabel(Command* command)
{
	assert(command != NULL);
	return command->ASM_cmd_code[command->ASM_cmd_len-1] == ':';
}

bool CheckValidOfCmdAndSetCPUCode(ASM* assembler, Command* command) 
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
		if(CheckCmdArgsValid(assembler, command, args_num))              \
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

void ASMCompileCommand(ASM* assembler, Command* command, size_t command_num)
{
	assert(assembler != NULL);
	assert(command != NULL);

	assembler->CS[command_num-1].cpu_comand = command->CPU_cmd_code;
	if (command->arguments_num > 0)
		assembler->CS[command_num-1].arg    = command->CPU_cmd_arg;

	#ifdef _DEBUG
	if (command->arguments_num > 0)
		fprintf(assembler->debug_compiled_file, "%d %lf", command->CPU_cmd_code, command->CPU_cmd_arg);
	else
		fprintf(assembler->debug_compiled_file, "%d", command->CPU_cmd_code);

	fprintf(assembler->debug_compiled_file, "\n");
	#endif
}

const int SPACE = 32;
size_t ReadWord(char* source_command_str, char* command_string) 
{
	int current_char_num = 0;
	while(source_command_str[current_char_num] != SPACE && source_command_str[current_char_num] != '\0')
		command_string[current_char_num] = source_command_str[current_char_num];

	return current_char_num;
}
size_t ReadExpInBrackets(char* source_command_str, Command* command)
{
	size_t read_words_num = 0;
	size_t current_char_num = 0;
	for (; source_command_str[current_char_num] != ']' && source_command_str[current_char_num] != '\0'; current_char_num++)
	{
		if(source_command_str[current_char_num] == SPACE)
			continue;
		else
		{
			ReadWord(&source_command_str[current_char_num], &command->ASM_cmd_arg[current_char_num]);
			read_words_num++;
		}
	}
	if (command->ASM_cmd_arg[current_char_num] == '\0')
		SetErrorBit(&(*(unsigned*)command->error), INVALID_SYNTAX);
}

void SkipSpaces(char* source_)


//ReadLine()
//{
//	int begin = SkipSpaces();
//	int end   = SkipNonSpaces();
//	strncpy (command->asm_cmd_code, source_command_str + begin, end-begin);
//
//	while ()
//	{
//		begin = SkipSpaces();
//		end   = SkipNonSpaces();
//		strncat (command->asm_cmd_arg, source_command_str + begin, end-begin);
//		}
//}


size_t ReadLine(char* source_command_str, Command* command)
{	
	size_t read_words_num = 0;
	int current_char_num = 0;
	while (source_command_str[current_char_num] != '\0')
	{
		if (source_command_str[current_char_num] == SPACE)
			SkipSpaces();
		else if (source_command_str[current_char_num] != SPACE && source_command_str[current_char_num] != '[')
		{
			if(read_words_num == 0)
				current_char_num += ReadWord(&source_command_str[current_char_num], command->ASM_cmd_code);
			else if(read_words_num == 1)
				current_char_num += ReadWord(&source_command_str[current_char_num], command->ASM_cmd_arg);
			read_words_num++;
		}
		else if (source_command_str[current_char_num] == '[')
		{
			if (read_words_num == 0)
				SetErrorBit(&(*(unsigned*)command->error), INVALID_SYNTAX);
			else if (read_words_num == 1)
				ReadExpInBrackets(&source_command_str[current_char_num], command);
			read_words_num++;
		}
		current_char_num ++;
	}
	return read_words_num;
}

void ASMProcessFile(ASM* assembler)
{
	assert(assembler != NULL);

	ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, BEFORE_PROCRESSING_FILE)

	#define CURRENT_COMMAND assembler->current_command

	size_t blank_lines_counter = 0;
	// FIRST PASSAGE
	for (size_t line_num = 1; line_num < assembler->text_info.strings_num + 1; line_num++)
	{
		char* source_command_str = assembler->text_info.text_strings[line_num - 1].str);

		ASM_CMD_SSCANF(source_command_str, CURRENT_COMMAND);

		if (CURRENT_COMMAND.arguments_num == -2)
			blank_lines_counter++;

		if (IsCommandLabel(&CURRENT_COMMAND))
		{
			if (CURRENT_COMMAND.arguments_num > 0)
				CURRENT_COMMAND.error = TOO_MANY_ARGS;

			strcpy(assembler->labels[assembler->labels_num].label_name, CURRENT_COMMAND.ASM_cmd_code);
			assembler->labels[assembler->labels_num].label_address = line_num - assembler->labels_num - blank_lines_counter;
			assembler->labels_num++;
		}

		ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, line_num)
	}
	// SECOND PASSAGE
	for (size_t line_num = 1; line_num < assembler->text_info.strings_num + 1; line_num++)
	{
		char* source_command_str = assembler->text_info.text_strings[line_num - 1].str;	
		
		ASM_CMD_SSCANF(source_command_str, CURRENT_COMMAND);

		if (CURRENT_COMMAND.arguments_num == -2)
			continue;

		if(CheckValidOfCmdAndSetCPUCode(assembler, &CURRENT_COMMAND))
			ASMCompileCommand(assembler, &CURRENT_COMMAND, line_num);

		ERROR_PROCESSING(assembler, ASMVerifier, ASMDump, ASMDtor, line_num)
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

	if (!assembler->compiled_file)
		SetErrorBit(&assembler->errors, ASM_COMPILED_FILE_ERROR);
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