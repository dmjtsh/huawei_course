#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include "..\..\DimasLIB\DimasTextInfo\text_info.h"
#include "..\..\DimasLIB\DimasStack\stack.h"

enum CPUErrors
{
	CPU_PTR_NULL                    = 1 << 0,
	CPU_BAD_STACK                   = 1 << 1,
	CPU_BAD_TEXT_INFO               = 1 << 2,
	CPU_WRONG_INPUT                 = 1 << 3,
	CPU_WRONG_COMMAND_USAGE         = 1 << 4,
	CPU_LOGGER_ERROR                = 1 << 5
};

const size_t BEFORE_PROCRESSING_FILE = 0; // Num of Line 

struct CPU
{
	Stack stack;
	TextInfo text_info;

	FILE* logger;

	Elem_t rax;
	Elem_t rbx;
	Elem_t rcx;
	Elem_t rdx;

	unsigned errors;
};

void CPUProcessFile(CPU* cpu);

void CPUDump(CPU* cpu, size_t num_of_line, FILE* logger);

int CPUVerifier(CPU* cpu);

int CPUCtor(CPU* cpu, const char* file_path);

int CPUDtor(CPU* cpu);

#endif // CPU_H