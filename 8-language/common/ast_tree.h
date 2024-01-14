#pragma once

enum AST_ERRORS
{
	INVALID_AST_FORMAT = 1 << 1
};

void ReadTree(Tree* tree, FILE* ast_file, unsigned* errors);
