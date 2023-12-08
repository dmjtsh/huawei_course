#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "tree.h"
#include "../DimasUtilities/error_processing.h"

TreeNode* OpNew(Tree* tree, TreeNode_t* data)
{
	assert(tree != nullptr);
	assert(data != nullptr);

	TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
	if (!node) return nullptr;

	tree->ElemCtor(&node->node_elem, data);

	return node;
}

void OpDelete(Tree* tree, TreeNode* node)
{
	assert(tree != nullptr);
	assert(node != nullptr);

	tree->ElemDtor(&node->node_elem);
}

void PrintNodePreOrder(Tree* tree, const TreeNode* node, FILE* logger)
{
	assert(tree   != nullptr);
	assert(logger != nullptr);

	if (!node) { fprintf(logger, " "); return; }
	
	fprintf(logger, "( ");

	fprintf(logger, "%s ", tree->ElemPrinter(&node->node_elem));
	PrintNodePreOrder(tree, node->left,  logger);
	PrintNodePreOrder(tree, node->right, logger);
	
	fprintf(logger, ") ");
}

void PrintNodeInOrder(Tree* tree, const TreeNode* node, FILE* logger)
{
	assert(tree   != nullptr);
	assert(logger != nullptr);

	if (!node) { fprintf(logger, " "); return; }
	
	fprintf(logger, "( ");
	
	PrintNodeInOrder(tree, node->left, logger);
	fprintf(logger, "%s ", tree->ElemPrinter(&node->node_elem));
	PrintNodeInOrder(tree, node->right, logger);
	
	fprintf(logger, ") ");
}

void PrintNodePastOrder(Tree* tree, const TreeNode* node, FILE* logger)
{
	assert(tree   != nullptr);
	assert(logger != nullptr);

	if (!node) { fprintf(logger, EMPTY_NODE " "); return; }
	
	fprintf(logger, "( ");
	
	PrintNodePastOrder(tree, node->left,  logger);
	PrintNodePastOrder(tree, node->right, logger);
	fprintf(logger, "%s ", tree->ElemPrinter(&node->node_elem));

	fprintf(logger, ") ");
}

TreeNode* CopyNode(Tree* tree, TreeNode* node)
{
	assert(tree != nullptr);

	if(!node)
		return nullptr;

	TreeNode* new_node = {};
	new_node = OpNew(tree, &node->node_elem);
	
	new_node->left  = CopyNode(tree, node->left);
	new_node->right = CopyNode(tree, node->right);

	return new_node;
}

void DestroyNode(Tree* tree, TreeNode* node)
{
	assert(tree != nullptr);

	if (!node)
		return;

	if(node->left)
		DestroyNode(tree, node->left);
	if(node->right)
		DestroyNode(tree, node->right);

	OpDelete(tree, node);
}

TreeNode* CreateNode(Tree* tree, TreeNode_t* data, TreeNode** node, TreeNode* left_node, TreeNode* right_node)
{
	assert(tree != nullptr);
	assert(node != nullptr);

	DestroyNode(tree, *node);

	*node = OpNew(tree, data);

	(*node)->left  = left_node;
	(*node)->right = right_node;

	return *node;
}

const char* VERTEX_COLOR = "#FFC61A";
const char* LEAFS_COLOR  = "#FF8100";
const char* LINKS_COLOR  = "#2E313F";
const char* FONT_COLOR   = "#0e0a2a";

void PrintGraphNode (Tree* tree, TreeNode* node, FILE* graph)
{
	assert(tree  != nullptr);
	assert(graph != nullptr);

	if(!node)
		return;

	if (node->left != nullptr && node->right != nullptr)
		fprintf(graph, 
			"node [shape=\"box\", style=\"filled\", fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.01\"];\n", 
			VERTEX_COLOR, FONT_COLOR);
	else
		fprintf(graph, 
			"node [shape=\"box\", style=\"filled\", fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.01\"];\n", 
			LEAFS_COLOR, FONT_COLOR);
	fprintf(graph,
		"\"%s\" [shape=\"record\", label=\"\\n %s\"];\n", tree->ElemPrinter(&node->node_elem), tree->ElemPrinter(&node->node_elem));
	
	PrintGraphNode(tree, node->left,  graph);
	PrintGraphNode(tree, node->right, graph);
}

void PrintGraphLinks (Tree* tree, TreeNode* node, FILE* graph)
{
	assert(tree != nullptr);
	assert(graph != nullptr);
	

	if (node->left != nullptr)
	{
		fprintf(graph, 
			"\"%s\" -> \"%s\"  [label = \"yes\" color=\"%s\" fontcolor=\"%s\"];\n",
			tree->ElemPrinter(&node->node_elem), tree->ElemPrinter(&node->left->node_elem), LINKS_COLOR, LINKS_COLOR);
		PrintGraphLinks(tree, node->left, graph);
	}
	
	if (node->right != nullptr)
	{
		fprintf(graph, 
			"\"%s\" -> \"%s\"  [label = \"no\" color=\"%s\" fontcolor=\"%s\"];\n",
			tree->ElemPrinter(&node->node_elem), tree->ElemPrinter(&node->right->node_elem), LINKS_COLOR, LINKS_COLOR);
		PrintGraphLinks(tree, node->right, graph);	
	}
}

void TreeGraphPrint(Tree* tree, FILE* graph)
{
	assert(tree  != nullptr);
	assert(graph != nullptr);

	fprintf(graph, "digraph TreeGraph {\n");

	PrintGraphNode (tree, tree->root, graph);
	PrintGraphLinks(tree, tree->root, graph);

	fprintf(graph, "}");
}

void TreeDump(Tree* tree, FILE* logger)
{
	assert(tree   != nullptr);
	assert(logger != nullptr);

	PrintNodeInOrder(tree, tree->root, logger);
}

unsigned TreeVerifier(Tree* tree)
{
	if (!tree)
		return TREE_PTR_NULL;

	CHECK_ERROR(tree, !tree->root, ROOT_PTR_NULL);

	return 0;
}

unsigned TreeCtor(Tree* tree, void  (*ElemCtor) (TreeNode_t*, TreeNode_t*), 
							  void  (*ElemDtor) (TreeNode_t*), 
							  char* (*ElemPrinter) (const TreeNode_t*))
{
	assert(tree != nullptr);
	assert(ElemCtor    != nullptr);
	assert(ElemDtor    != nullptr);
	assert(ElemPrinter != nullptr);

	tree->ElemCtor    = ElemCtor;
	tree->ElemDtor    = ElemDtor;
	tree->ElemPrinter = ElemPrinter;

	return 0;
}

unsigned TreeDtor(Tree* tree)
{
	assert(tree != nullptr);

	if (!(tree->errors & TREE_DELETED))
	{
		DestroyNode(tree, tree->root);
		tree->errors |= TREE_DELETED;
	}
	else
		return TREE_DELETED;

	return 0;
}