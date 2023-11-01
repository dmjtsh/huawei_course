#include <stdio.h>
#include <assert.h>

#include "commands.h"


void CommandDump(Command* command, FILE* file_to_dump)
{
	if (command->error & INVALID_SYNTAX)     { fprintf(file_to_dump, "Invalid Syntax!\n");                              return; } 
	if (command->error & INVALID_REG_NAME)   { fprintf(file_to_dump, "Invalid reg name!\n");                            return; } 
	if (command->error & TOO_MANY_ARGS)      { fprintf(file_to_dump, "Too MANY args in command!\n");                    return; }
	if (command->error & TOO_FEW_ARGS)       { fprintf(file_to_dump, "Too FEW args in command!\n");                     return; }   
	if (command->error & POP_WITH_NUM)       { fprintf(file_to_dump, "You just did Pop with num, are you crazy????\n"); return; }
}

void SetCmdBitCode(CPUCommand* command_cpu_code, CmdArgType arg_type)
{
	assert(command_cpu_code != NULL);

	*(char*)command_cpu_code |= (char)arg_type;
}

void UnsetCmdBitCode(CPUCommand* command_cpu_code, CmdArgType arg_type)
{
	assert(command_cpu_code != NULL);

	*(char*)command_cpu_code &= ~(char)arg_type;
}