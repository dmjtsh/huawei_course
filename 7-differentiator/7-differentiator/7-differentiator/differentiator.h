#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "DimasLIB/DimasTree/tree.h"
#include "DimasLIB/DimasTextInfo/text_info.h"
#include "DimasLIB/DimasUtilities/utilities.h"

const size_t MAX_COMMAND_LENGTH = 100;

struct Differentiator
{
	Tree tree;

	unsigned errors;
	TextInfo text_info;
};

void PrintLatexDiffNode(FILE* file_to_print, Differentiator* diff, TreeNode* node, TreeNode* parent_node);
TreeNode* Differentiate(Differentiator* diff, TreeNode* node);

unsigned DifferentiatorDtor(Differentiator* diff);
unsigned DifferentiatorCtor(Differentiator* diff);

#endif DIFFERENTIATOR_H // DIFFERENTIATOR_H