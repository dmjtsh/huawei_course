#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

#include "elems_type.h"

const size_t MAX_COMMAND_LENGTH     = 20;
const size_t MAX_ARG_LENGTH         = 30;
const size_t COMMANDS_NUM           = 10;
const size_t COUNT_OF_CMD_ARG_TYPES = 4;

enum CommandArgType { NUMBER_TYPE   = 1 << 5, 
				      REGISTER_TYPE = 1 << 6,
				      MEMORY_TYPE   = 1 << 7};

/*
* COMMANDS DEFINITIONS BLOCK
*/

enum CPUCommand 
{
	#define CMD_DEF(name, cpu_code, ...) name = cpu_code,

	#include "cmds_defs.h",
	INVALID_CPU_COMMAND

	#undef CMD_DEF
};

const char* const ASM_COMMANDS[] =
{
	#define CMD_DEF(name, ...) #name,

	#include "cmds_defs.h"

	#undef CMD_DEF
};

/*
* END OF COMMANDS DEFINITIONS BLOCK
*/

/*
* COMMANDS STRUCTS BLOCK
*/

struct CPUCommandWithArg // for CS
{
	CPUCommand cmd;
	Elem_t arg;
};

enum CommandError;
struct Command
{
	char ASM_cmd_code[MAX_COMMAND_LENGTH];
	char ASM_cmd_arg[MAX_ARG_LENGTH];
	
	CPUCommandWithArg CPU_cmd_with_arg;

	int arguments_num;
	CommandArgType cmd_arg_type;
	CommandError error;
};



/*
* END OF COMMANDS STRUCTS BLOCK
*/

/*
* COMMAND ERRORS BLOCK
*/

void SetCommandBitCode(CPUCommand* command_cpu_code, CommandArgType arg_type);
void UnsetCommandBitCode(CPUCommand* command_cpu_code, CommandArgType arg_type);

void SetCommandTypeBitCode(CommandArgType* old_arg_type, CommandArgType new_arg_type);
void UnsetCommandTypeBitCode(CommandArgType* old_arg_type, CommandArgType new_arg_type);

enum CommandError
{
	ALL_OK                    = 0,
	INVALID_SYNTAX            = 1 << 1,
	TOO_MANY_ARGS             = 1 << 2,
	TOO_FEW_ARGS              = 1 << 3,
	INVALID_REG_OR_LABEL_NAME = 1 << 4,
	POP_WITH_NUM              = 1 << 5,
	TOO_BIG_COMMAND           = 1 << 6,
	TOO_BIG_ARG               = 1 << 7
};

void CommandDump(Command* command, FILE* file_to_dump);

/*
* END OF COMMAND ERRORS BLOCK
*/

#endif 