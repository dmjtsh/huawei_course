#include "commands.h"
#include <stdio.h>

bool IsValidCommand(Command* command)
{
	if (!command)
		return false;

	switch (command->CPU_cmd_code)
	{
		#define CMD_DEF(name, cpu_code, num_of_args, ...)                \
		case name:                                                       \
			if(num_of_args != command->arguments_num)                    \
				return false;                                            \
			else                                                         \
				return true;                                             \
	
		#include "cmds_defs.h"
		default:
			return false;
	}
	#undef CMD_DEF
	return true;
}
