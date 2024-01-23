#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "../../common/DimasLIB/DimasTree/tree.h"
#include "../../common/nametable.h"
#include "syntax_analizer.h"

TreeNode** DoLexicalAnalisys(Tree* expr_tree, const char* file_name, NameTable* nametable);

char*     GetOperDesignation(Operation oper);
Operation GetOperValue(const char* design);
bool      IsOper(const char* design);

TreeNode* CreateNumNode (Tree* expr_tree, double num);
TreeNode* CreateOperNode(Tree* expr_tree, Operation oper, TreeNode* left_node, TreeNode* right_node);
TreeNode* CreateVarNode(Tree* expr_tree, NameTableElem* nametable_elem_p);

#endif