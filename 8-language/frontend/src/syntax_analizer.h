#pragma once

#include "../../common/DimasLIB/DimasTree/tree.h"
#include "../../common/nametable.h"

char* ExprTreeElemPrinter(const TreeNode_t* elem_to_print);
void  ExprTreeElemCtor(TreeNode_t* new_elem, TreeNode_t* new_data);
void  ExprTreeElemDtor(TreeNode_t* elem_to_delete);

void PrintExprNode(TreeNode* node);

Tree GetCodeTree(const char* file_name, NameTable* nametable);