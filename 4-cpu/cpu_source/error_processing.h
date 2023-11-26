#define CHECK_ERROR(obj, condition, error)  \
	if(condition)						    \
		SetErrorBit(&obj->errors, error);   \
	else								    \
		UnsetErrorBit(&obj->errors, error);

#define ERROR_PROCESSING(structure, StructVerifier, StructDump, StructDtor, line_of_file) \
	if (StructVerifier(structure))														  \
	{																					  \
		StructDump(structure, line_of_file, stderr);									  \
		StructDtor(structure);														      \
		return structure->errors;														  \
	}																					  \
	StructDump(structure, line_of_file, structure->logger);								  