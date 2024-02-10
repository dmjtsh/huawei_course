#pragma once

#include "program_nametables.h"
#include "DimasLIB/DimasTree/tree.h"

enum AST_ERRORS
{
	INVALID_AST_FORMAT = 1 << 1
};

char* ASTTreeElemPrinter(const TreeNode_t* elem_to_print);
void  ASTTreeElemCtor(TreeNode_t* new_elem, TreeNode_t* new_data);
void  ASTTreeElemDtor(TreeNode_t* elem_to_delete);

void ReadASTTree(Tree* tree, ProgramNameTables* nametables, FILE* ast_file, unsigned* errors);
void WriteASTTree(Tree* tree, FILE* logger);
