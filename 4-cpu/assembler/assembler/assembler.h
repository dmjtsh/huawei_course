#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../../DimasLIB/DimasTextInfo/text_info.h"
#include "../../DimasLIB/DimasUtilities/utilities.h"
#include "../../commands.h"
#include "../../elems_type.h"

const size_t LABELS_ARR_SIZE = 50;
struct Label 
{
	char  label_name[MAX_ARG_LENGTH];
	int   label_address;
};

enum ASMErrors
{
	ASM_PTR_NULL              = 1 << 0,
	ASM_BAD_TEXT_INFO         = 1 << 2,
	INVALID_ASM_COMMAND       = 1 << 3,
	ASM_LOGER_ERROR           = 1 << 4,
	ASM_COMPILED_FILE_ERROR   = 1 << 5,
	ASM_CURRENT_COMMAND_ERROR = 1 << 6
};

struct ASM
{
	TextInfo text_info;

	FILE* logger;
	unsigned errors;

	Command current_command;
	CpuCommandWithArg* CS; 

	Label labels[LABELS_ARR_SIZE];
	size_t labels_num;

	FILE* compiled_file;
	#ifdef _DEBUG
	FILE* debug_compiled_file;
	#endif
};

void ASMProcessFile(ASM* assembler);

void ASMDump(ASM* assembler, size_t num_of_line, FILE* logger);

const size_t BEFORE_PROCRESSING_FILE = 0; // Num of Line for Verifier
int ASMVerifier(ASM* assembler);

int ASMCtor(ASM* assembler, const char* file_path, const char* compiled_file_path);

int ASMDtor(ASM* assembler);

#endif