#include <stdio.h>
#include <assert.h>

#include "commands.h"


void CommandDump(Command* command, FILE* file_to_dump)
{
	fprintf(file_to_dump, "Cmd: %s %s\n", command->ASM_cmd_code, command->ASM_cmd_arg);
	if (command->error & INVALID_SYNTAX)              { fprintf(file_to_dump, "Invalid Syntax!\n");							     return; } 
	if (command->error & INVALID_REG_OR_LABEL_NAME)   { fprintf(file_to_dump, "Invalid reg or label name!\n");				     return; } 
	if (command->error & TOO_MANY_ARGS)               { fprintf(file_to_dump, "Too MANY args in command!\n");				     return; }
	if (command->error & TOO_FEW_ARGS)                { fprintf(file_to_dump, "Too FEW args in command!\n");				     return; }   
	if (command->error & POP_WITH_NUM)                { fprintf(file_to_dump, "You just did Pop with num, are you crazy????\n"); return; }
}

void SetCommandBitCode(CPUCommand* command_cpu_code, CommandArgType arg_type)
{
	assert(command_cpu_code != NULL);

	*(short*)command_cpu_code |= (short)arg_type;
}

void UnsetCommandBitCode(CPUCommand* command_cpu_code, CommandArgType arg_type)
{
	assert(command_cpu_code != NULL);

	*(short*)command_cpu_code &= ~(short)arg_type;
}

void SetCommandTypeBitCode(CommandArgType* old_arg_type, CommandArgType new_arg_type)
{
	assert(old_arg_type != NULL);

	*(short*)old_arg_type |= (short)new_arg_type;
}

void UnsetCommandTypeBitCode(CommandArgType* old_arg_type, CommandArgType new_arg_type)
{
	assert(old_arg_type != NULL);

	*(short*)old_arg_type &= ~(short)new_arg_type;
}