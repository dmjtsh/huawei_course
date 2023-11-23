#define CHECK_ERROR(obj, condition, error)  \
	if(condition)						    \
		obj->errors |= error;               \
	else								    \
		obj->errors &= ~error;

const unsigned WRONG_COMMAND_USAGE = -1;

#define ERROR_PROCESSING(structure, StructVerifier, StructDump, StructDtor) \
	if (StructVerifier(structure))											\
	{																		\
		StructDump(structure, stderr);						                \
		StructDtor(structure);												\
		return structure->errors;											\
	}																		\
	StructDump(structure, structure->logger);