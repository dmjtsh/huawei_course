#define _CRT_SECURE_NO_WARNINGS

#include <string.h>

#include "operation.h"

OperPriority GetOperPriority(Operation oper)
{
	OperPriority priority = LAST_PRIORITY;

	if(0);
	#define OPER_DEF(value, designation, oper_priority) \
	else if(oper == value)                              \
		priority = oper_priority;

	#include "opers_defs.h"

	#undef OPER_DEF

	return priority;
}

char* GetOperDesignation(Operation oper)
{
	static char design[MAX_OPER_LEN] = "";

	if(0);
	#define OPER_DEF(value, designation, ...) \
	else if(oper == value)                    \
		strcpy(design, designation);

	#include "opers_defs.h"

	#undef OPER_DEF

	return design;
}

OperNumArgs GetOperNumArgs(Operation oper)
{
	OperNumArgs oper_num_args = NO_ARGS;
	
	if(0);
	#define OPER_DEF(value, designation, priority, operation_num_args, latex_printing_code, ...) \
	else if(oper == value)                                                                       \
		oper_num_args = operation_num_args;

	#include "opers_defs.h"

	#undef OPER_DEF

	return oper_num_args;
}
