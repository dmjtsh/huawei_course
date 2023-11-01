#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

#include "elems_type.h"

const size_t MAX_COMMAND_LENGTH     = 10;
const size_t MAX_ARG_LENGTH         = 20;
const size_t COMMANDS_NUM           = 10;
const size_t COUNT_OF_CMD_ARG_TYPES = 4;

enum CmdArgType { NUMBER_TYPE   = 1 << 5, 
				  REGISTER_TYPE = 1 << 6,
				  MEMORY_TYPE   = 1 << 7,
				  LABEL_TYPE    = 1 << 8,
				  WRONG_TYPE    = 1 << 9};

/*
* COMMANDS DEFINITIONS BLOCK
*/

#define CMD_DEF(name, cpu_code, ...) name = cpu_code,
enum CPUCommand 
{
	#include "cmds_defs.h"
	INVALID_CPU_COMMAND = 1337
};
#undef CMD_DEF

#define CMD_DEF(name, ...) #name,
const char* const ASM_COMMANDS[] =
{
	#include "cmds_defs.h"
};
#undef CMD_DEF

/*
* END OF COMMANDS DEFINITIONS BLOCK
*/

/*
* COMMANDS STRUCTS BLOCK
*/

enum CommandError;
struct Command
{
	char ASM_cmd_code[MAX_COMMAND_LENGTH];
	char ASM_cmd_arg[MAX_ARG_LENGTH];
	size_t asm_cmd_len;
	size_t asm_cmd_arg_len;
	
	CPUCommand CPU_cmd_code;
	Elem_t CPU_cmd_arg;

	size_t arguments_num;
	CmdArgType cmd_arg_type;
	CommandError error;
};

struct CpuCommandWithArg
{
	CPUCommand cpu_comand;
	double arg;
};

/*
* END OF COMMANDS STRUCTS BLOCK
*/

/*
* COMMAND ERRORS BLOCK
*/

void SetCmdBitCode(CPUCommand* command_cpu_code, CmdArgType arg_type);

void UnsetCmdBitCode(CPUCommand* command_cpu_code, CmdArgType arg_type);

enum CommandError
{
	ALL_OK           = 0,
	INVALID_SYNTAX   = 1 << 1,
	TOO_MANY_ARGS    = 1 << 2,
	TOO_FEW_ARGS     = 1 << 3,
	INVALID_REG_NAME = 1 << 4,
	POP_WITH_NUM     = 1 << 5
};

void CommandDump(Command* command, FILE* file_to_dump);

/*
* END OF COMMAND ERRORS BLOCK
*/

#endif 