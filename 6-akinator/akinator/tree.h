#ifndef TREE_H
#define TREE_H

#include <stdio.h>

/*
* Tree Node Structure Block
*/

#define EMPTY_NODE "nil" 
const size_t MAX_NODE_STR_LEN = 60;

struct Tree;

typedef char TreeNode_t[MAX_NODE_STR_LEN];

struct TreeNode
{
	TreeNode_t elem;

	TreeNode* left;
	TreeNode* right;
};

TreeNode* OpNew(Tree* tree, TreeNode_t* data);
void      OpDelete(Tree* tree, TreeNode* node);

void PrintNodePastOrder(Tree* tree, const TreeNode* node, FILE* logger);
void PrintNodeInOrder  (Tree* tree, const TreeNode* node, FILE* logger);
void PrintNodePreOrder (Tree* tree, const TreeNode* node, FILE* logger);

void DestroyNode(Tree* tree, TreeNode* node_ptr);

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

	void  (*ElemCtor) (TreeNode_t*, TreeNode_t*);
	void  (*ElemDtor) (TreeNode_t*);
	char* (*ElemPrinter) (const TreeNode_t*);

	unsigned errors;
};

unsigned TreeCtor(Tree* tree, void  (*ElemCtor) (TreeNode_t*, TreeNode_t*), 
							  void  (*ElemDtor) (TreeNode_t*), 
							  char* (*ElemPrinter) (const TreeNode_t*));
unsigned TreeDtor(Tree* tree);

unsigned TreeVerifier(Tree* tree);
void TreeDump(Tree* tree, FILE* logger);
void TreeGraphPrint(Tree* tree, FILE* graph);
/*
* End of Tree Structure Block 
*/

#endif