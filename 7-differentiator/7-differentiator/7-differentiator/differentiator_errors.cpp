#include <assert.h>
#include <stdio.h>

#include "logger.h"
#include "differentiator.h"
#include "differentiator_errors.h"
#include "DimasLIB/DimasUtilities/error_processing.h"

void DifferentiatorDump(Differentiator* diff)
{
	assert(diff != NULL);

	static size_t num_of_call = 1;
	LOG("=======================================\n"
		"AKINATOR DUMP CALL #%zu\n", num_of_call);
	if (diff->errors)
	{
		LOG("-------------ERRORS------------\n");
		if (diff->errors & DIFF_PTR_NULL)
		{
			LOG("DIFFERENTIATOR POINTER IS NULL\n");
			return;
		}
		if (diff->errors & DIFF_TREE_ERROR)      LOG("AKINATOR TREE ERROR\n");
		if (diff->errors & DIFF_TEXT_INFO_ERROR) LOG("AKINATOR TEXT INFO ERROR\n");

		LOG("----------END_OF_ERRORS--------\n");
	}
	else
	{
		LOG("------------NO_ERRORS----------\n");

		LOG("Akinator Tree:\n\n");

		// TODO ADD NORMAL TREE DUMP 
		//TreeDump(&diff->tree); 

		LOG("\n\n");
	}
	
	LOG("=======================================\n\n");

	num_of_call++;
}							  


unsigned DifferentiatorVerifier(Differentiator* diff)
{
	if (!diff)
		return DIFF_PTR_NULL;

	TreeVerifier(&diff->tree);

	CHECK_ERROR(diff, diff->tree.errors,            TREE_PTR_NULL);
	CHECK_ERROR(diff, !diff->text_info.is_okay,     DIFF_TEXT_INFO_ERROR);

	return diff->errors;
}