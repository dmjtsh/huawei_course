#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include "../../operation.h"

/*
* Tree Node Structure Block
*/

#define EMPTY_NODE "." 

enum TreeElemType { NUM, OPER, VAR };

struct Variable
{
	double value;
	char   name;
};

union TreeNodeElem
{
	double   num;
	Operation oper;
	Variable var;
};

struct TreeNode_t
{
	TreeNodeElem elem;
	TreeElemType type;
};

struct TreeNode
{
	TreeNode_t node_elem;

	TreeNode* left;
	TreeNode* right;
};

struct Tree;

TreeNode* OpNew(Tree* tree, TreeNode_t* data);
void      OpDelete(Tree* tree, TreeNode* node);

void PrintNodePastOrder(Tree* tree, const TreeNode* node, FILE* logger);
void PrintNodeInOrder  (Tree* tree, const TreeNode* node, FILE* logger);
void PrintNodePreOrder (Tree* tree, const TreeNode* node, FILE* logger);

TreeNode*   CopyNode(Tree* tree, TreeNode* node);
TreeNode* CreateNode(Tree* tree, TreeNode_t* data, TreeNode** node, TreeNode* left_node, TreeNode* right_node);
void      DestroyNode(Tree* tree, TreeNode* node);

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
	TREE_DELETED      = 1 << 3
};

struct Tree
{
	TreeNode* root;

	void  (*ElemCtor)    (TreeNode_t*, TreeNode_t*);
	void  (*ElemDtor)    (TreeNode_t*);
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