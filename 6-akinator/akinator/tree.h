#ifndef TREE_H
#define TREE_H

#include <stdio.h>

/*
* Tree Node Structure Block
*/

#define EMPTY_NODE "nil" 
const size_t MAX_NODE_STR_LEN = 60;

using TreeNode_t = char[MAX_NODE_STR_LEN];

struct TreeNode
{
	TreeNode_t elem;

	TreeNode* left;
	TreeNode* right;
};

TreeNode* OpNew(TreeNode_t data);
void      OpDelete(TreeNode* node);

void   PrintNode(const TreeNode* node, FILE* logger);
size_t ReadNode(TreeNode** node_ptr, const char* tree_text_repr, unsigned* errors_ptr);

void DestroyNode(TreeNode** node_ptr);

/*
* End of Node Structure Block
*/


/*
* Tree Structure Block
*/


enum TreeErrorBits
{
	TREE_PTR_NULL     = 1 << 0,
	ROOT_PTR_NULL     = 1 << 1,
	NODE_CALLOC_ERROR = 1 << 2,
	TREE_STR_INVALID  = 1 << 6,
	TREE_DELETED      = 1 << 9
};

struct Tree
{
	TreeNode* root;

	unsigned errors;
};

unsigned TreeCtor(Tree* tree, char* tree_text_repr);
unsigned TreeDtor(Tree* tree);

unsigned TreeVerifier(Tree* tree);
void     TreeDump(Tree* tree, FILE* logger);

/*
* End of Tree Structure Block 
*/

#endif