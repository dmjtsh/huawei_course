#ifndef OPERATION_H
#define OPERATION_H

const size_t MAX_OPER_LEN = 4;

enum OperNumArgs { NO_ARGS = 0, ONE_ARG = 1, TWO_ARGS = 2 };

enum OperPriority { FIRST_PRIORITY = 3, SECOND_PRIORITY = 2, THIRD_PRIORITY = 1, LAST_PRIORITY = 0 };

enum Operation 
{
	#define OPER_DEF(name, ...) name,

	#include "../../opers_defs.h"

	#undef CMD_DEF
};

OperPriority GetOperPriority(Operation oper);
OperNumArgs  GetOperNumArgs(Operation oper);
char*        GetOperDesignation(Operation oper);

#endif