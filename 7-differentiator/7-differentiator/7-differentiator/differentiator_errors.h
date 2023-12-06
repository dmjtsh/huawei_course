#ifndef DIFFERENTIATOR_ERRORS_H
#define DIFFERENTIATOR_ERRORS_H

#include <stdio.h>

#include "differentiator.h"

enum DifferentiatorErrorBits
{
	DIFF_PTR_NULL            = 1 << 0,
	DIFF_TREE_ERROR          = 1 << 1,
	DIFF_LOGGER_ERROR        = 1 << 2,
	DIFF_GRAPH_ERROR         = 1 << 3,
	DIFF_TEXT_INFO_ERROR     = 1 << 4,
	DIFF_DELETED             = 1 << 5,
	DIFF_FILE_INVALID_SYNTAX = 1 << 6
};

void DifferentiatorDump(Differentiator* diff, FILE* logger);
unsigned DifferentiatorVerifier(Differentiator* diff);

#endif // DIFFERENTIATOR_ERRORS_H