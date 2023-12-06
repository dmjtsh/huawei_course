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

	free(node);
}

void PrintNodePreOrder(Tree* tree, const TreeNode* node, FILE* logger)
{
	assert(tree   != nullptr);
	assert(logger != nullptr);

	if (!node) { fprintf(logger, EMPTY_NODE " "); return; }
	
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

	if (!node) { fprintf(logger, EMPTY_NODE " "); return; }
	
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

void CreateChildNode(Tree* tree, TreeNode* parent_node, TreeNode_t* child_node_data)
{
	assert(tree != nullptr);
	assert(parent_node != nullptr);
	assert(child_node_data != nullptr);

	if (!parent_node->left)
		parent_node->left  = OpNew(tree, child_node_data);
	else if(!parent_node->right)
		parent_node->right = OpNew(tree, child_node_data); 
}

void DeleteLeaf(Tree* tree, TreeNode* leaf_to_delete)
{
	assert(tree != nullptr);
	assert(leaf_to_delete != nullptr);

	if(!leaf_to_delete->left && !leaf_to_delete->right)
		OpDelete(tree, leaf_to_delete);
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