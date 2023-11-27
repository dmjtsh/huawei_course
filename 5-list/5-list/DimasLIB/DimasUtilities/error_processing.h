#ifndef ERROR_PROCESSING_H
#define ERROR_PROCESSING_H

#define CHECK_ERROR(obj, condition, error)  \
do										    \
{											\
	if(condition)						    \
		(obj)->errors |= error;             \
	else								    \
		(obj)->errors &= ~(error);			\
} while(0)

const unsigned WRONG_COMMAND_USAGE = -1;

#define ERROR_PROCESSING(structure, StructVerifier, StructDump, StructDtor) \
do																			\
{																			\
	if (StructVerifier(structure))											\
	{																		\
		StructDump(structure, stderr);						                \
		StructDtor(structure);												\
		return (structure)->errors;											\
	}																		\
	StructDump(structure, (structure)->logger);								\
} while(0)

#endif