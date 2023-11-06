#ifndef CPU_H
#define CPU_H

#include <stdio.h>

#include "../../cpu_source/commands.h"
#include "../../DimasLIB/DimasTextInfo/text_info.h"
#include "../../DimasLIB/DimasStack/stack.h"


const size_t BEFORE_PROCRESSING_FILE = 0; // Num of Line 
enum CPUErrors
{
	CPU_PTR_NULL              = 1 << 0,
	CPU_BAD_STACK             = 1 << 1,
	CPU_WRONG_INPUT           = 1 << 2,
	CPU_WRONG_COMMAND_USAGE   = 1 << 3,
	CPU_LOGER_ERROR           = 1 << 4,
	CPU_CURRENT_COMMAND_ERROR = 1 << 5
};

const size_t RAM_SIZE = 100;
struct CPU
{
	Stack stack;

	CPUCommandWithArg* CS; 
	size_t current_line_num;
	size_t commands_num;
	Command current_command;

	FILE* compiled_file;

	FILE* logger;
	unsigned errors;

	Elem_t RAM[RAM_SIZE];

	#define REG_DEF(name, ...) Elem_t name;

	#include "../../cpu_source/regs_defs.h"

	#undef REG_DEF
};

enum RegisterCode
{
	#define REG_DEF(name, cpu_code) name = cpu_code,

	#include "../../cpu_source/regs_defs.h"

	#undef REG_DEF
};

void   SetReg(CPU* cpu, Elem_t reg, Elem_t value);

Elem_t GetReg(CPU* cpu, Elem_t reg);

void CPUProcessFile(CPU* cpu);

void CPUDump(CPU* cpu, size_t num_of_line, FILE* logger);

int  CPUVerifier(CPU* cpu);

int  CPUCtor(CPU* cpu, const char* file_path);

int  CPUDtor(CPU* cpu);

#endif // CPU_H