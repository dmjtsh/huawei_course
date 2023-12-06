#define _CRT_SECURE_NO_WARNINGS

#include "differentiator.h"

int main()
{
	Differentiator diff = {};

	DifferentiatorCtor(&diff);

	FILE* file = fopen("nedoteh.txt", "w");
	PrintLatexDiffNode(stdout, &diff, diff.tree.root, nullptr);
	printf("\n");
	Differentiate(&diff, diff.tree.root);
	PrintLatexDiffNode(stdout, &diff, diff.tree.root, nullptr);
	printf("\n");
}