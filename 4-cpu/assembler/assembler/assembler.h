#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../../DimasLIB/DimasTextInfo/text_info.h"
#include "../../DimasLIB/DimasUtilities/utilities.h"
#include "../../commands.h"
#include "../../elems_type.h"

enum ASMErrors
{
	ASM_PTR_NULL                    = 1 << 0,
	ASM_BAD_TEXT_INFO               = 1 << 2,
	ASM_WRONG_INPUT                 = 1 << 3,
	ASM_WRONG_COMMAND_USAGE         = 1 << 4,
	ASM_LOGER_ERROR                 = 1 << 5,
	ASM_COMPILED_FILE_ERROR         = 1 << 6
};

const size_t BEFORE_PROCRESSING_FILE = 0; // Num of Line 

struct ASM
{
	TextInfo text_info;

	FILE* logger;
	unsigned errors;

	FILE* compiled_file;
};

void ASMProcessFile(ASM* assembler);

void ASMDump(ASM* assembler, size_t num_of_line, FILE* logger);

int ASMVerifier(ASM* assembler);

int ASMCtor(ASM* assembler, const char* file_path, const char* compiled_file_path);

int ASMDtor(ASM* assembler);

#endif