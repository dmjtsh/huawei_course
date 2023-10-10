#ifndef COMMANDS_H
#define COMMANDS_H

#include "elems_type.h"

#define CMD_DEF(name, cpu_code, ...) name = cpu_code,
enum CPUCommand 
{
	#include "cmds_defs.h"
};
#undef CMD_DEF

#define CMD_DEF(name, ...) #name,
const char* const ASMCommands[] =
{
	#include "cmds_defs.h"
};
#undef CMD_DEF

const size_t MAX_COMMAND_LENGTH = 4;
const size_t COMMANDS_NUM       = 10;

struct Command
{
	char ASM_cmd_code[MAX_COMMAND_LENGTH] = {};
	CPUCommand CPU_cmd_code = {};

	Elem_t cmd_arg = {};

	size_t arguments_num = 0;
};

bool IsValidCommand(Command* command);

#endif 