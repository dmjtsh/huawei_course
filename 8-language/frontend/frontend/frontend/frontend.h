#ifndef FRONTEND_H
#define FRONTEND_H

#include "DimasLIB/DimasTree/tree.h"

const size_t MAX_NAMES_TABLE_SIZE = 100;

struct NameTableElem
{
	TreeElemType type;
	const char*  str;
	int          code;
};

struct NameTable
{
	NameTableElem elems[MAX_NAMES_TABLE_SIZE];
	size_t size;
};

Tree GetCodeTree(const char* file_name);

NameTableElem* NameTableFind(NameTable* name_table, const char* elem_to_find);

TreeNode* CreateNumNode (Tree* expr_tree, double num);
TreeNode* CreateOperNode(Tree* expr_tree, Operation oper, TreeNode* left_node, TreeNode* right_node);
TreeNode* CreateVarNode (Tree* expr_tree, char* var_name);

#endif