#pragma once

#include "../../TX/TXLib.h"
#include "../../nametable.h"

#include <stdio.h>

/*
* Tree Node Structure Block
*/

const int MAX_OPER_LEN = 50;

enum Operation
{
	#define OPER_DEF(code, ...) code,
	
	#include "../../opers_defs.h"

	#undef OPER_DEF

	OPER_COUNT,
	WRONG_OPER
};

#define EMPTY_NODE "." 

enum TreeElemType { NUM, OPER, ID};

union TreeNodeElem
{
	double         num;
	Operation      oper;
	NameTableElem* id;
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

void PrintNodePreOrder (Tree* tree, const TreeNode* node, FILE* logger);
void PrintNodePostOrder(Tree* tree, const TreeNode* node, FILE* logger);
void PrintNodeInOrder  (Tree* tree, const TreeNode* node, FILE* logger);

void WriteNodePreOrder(Tree* tree, const TreeNode* node, FILE* logger, int rec_depth);

TreeNode* CopyNode(Tree* tree, TreeNode* node);
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

unsigned TreeCtor(Tree* tree, void  (*ElemCtor)    (TreeNode_t*, TreeNode_t*), 
							  void  (*ElemDtor)    (TreeNode_t*), 
							  char* (*ElemPrinter) (const TreeNode_t*));
unsigned TreeDtor(Tree* tree);

unsigned TreeVerifier(Tree* tree);
void     TreeGraphPrint(Tree* tree, const char* file_name);

/*
* End of Tree Structure Block 
*/