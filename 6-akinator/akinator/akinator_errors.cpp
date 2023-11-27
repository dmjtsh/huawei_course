#include <stdio.h>

#include "DimasLIB/DimasUtilities/error_processing.h"
#include "akinator.h"
#include "akinator_errors.h"

void AkinatorDump(Akinator* akinator, FILE* logger)
{
	assert(akinator != NULL);
	assert(logger != NULL);

	static size_t num_of_call = 1;
	fprintf(logger, 
	"=======================================\n"
	"AKINATOR DUMP CALL #%zu\n", num_of_call);
	if (akinator->errors)
	{
		fprintf(logger,
		"-------------ERRORS------------\n");
		if (akinator->errors & AKINATOR_PTR_NULL)
		{
			fprintf(logger, "AKINATOR POINTER IS NULL\n");
			return;
		}
		if (akinator->errors & AKINATOR_LOGGER_ERROR)    fprintf(logger, "AKINATOR LOGGER ERROR\n");
		if (akinator->errors & AKINATOR_TREE_ERROR)      fprintf(logger, "AKINATOR TREE ERROR\n");
        if (akinator->errors & AKINATOR_GRAPH_ERROR)     fprintf(logger, "AKINATOR GRAPH ERROR\n");
		if (akinator->errors & AKINATOR_TEXT_INFO_ERROR) fprintf(logger, "AKINATOR TEXT INFO ERROR\n");
		if (akinator->errors & PROPERTY_STACK_ERROR)     fprintf(logger, "PROPERTY STACK ERROR\n");

		fprintf(logger,
		"----------END_OF_ERRORS--------\n");
	}
	else
	{
		fprintf(logger,
		"------------NO_ERRORS----------\n");

		fprintf(logger, "Akinator Tree:\n\n");

		TreeDump(&akinator->tree, logger);

		fprintf(logger, "\n\n");
	}
	
	fprintf(logger,
	"=======================================\n\n");

	num_of_call++;
}							  


unsigned AkinatorVerifier(Akinator* akinator)
{
	if (!akinator)
		return AKINATOR_PTR_NULL;

	TreeVerifier(&akinator->tree);

	CHECK_ERROR(akinator, akinator->tree.errors,            TREE_PTR_NULL);
	CHECK_ERROR(akinator, akinator->logger == nullptr,      AKINATOR_LOGGER_ERROR);
	CHECK_ERROR(akinator, akinator->logger == nullptr,      AKINATOR_GRAPH_ERROR);
	CHECK_ERROR(akinator, !akinator->text_info.is_okay,     AKINATOR_TEXT_INFO_ERROR);
	CHECK_ERROR(akinator, akinator->node_properties.errors, PROPERTY_STACK_ERROR);

	return akinator->errors;
}