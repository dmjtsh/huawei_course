#include <assert.h>

#include "differentiator.h"
#include "differentiator_io.h"

extern IntermediatePrinter INTERMEDIATE_PRINTER;

#define DESTROY_UNNECESSARY_NODE_IF_NEEDED(what_to_copy, unnecessary_node, unnecessary_node_expected_val)  \
	do                                                                                                     \
	{                                                                                                      \
		if(unnecessary_node->node_elem.type == NUM												           \
		   && IsDoubleNumsEqual(unnecessary_node->node_elem.elem.num, unnecessary_node_expected_val))      \
		{																						           \
			node_to_destroy = *node;																	   \
			*node = COPY(what_to_copy);																	   \
			DestroyNode(expr_tree, node_to_destroy);												       \
			*is_node_changed = true;																       \
		}																								   \
	} while(0)

void DeleteUnnecessaryNodeExpr(Tree* expr_tree, TreeNode** node, bool* is_node_changed)
{
	assert(expr_tree       != nullptr);
	assert(node			   != nullptr);
	assert(is_node_changed != nullptr);

	TreeNode* node_to_destroy = nullptr;

	if (!*node || !(*node)->left || !(*node)->right)
		return;

	TreeNode* node_left          = (*node)->left;
	TreeNode* node_right         = (*node)->right;
	Operation node_oper          = (*node)->node_elem.elem.oper;

	#define IS(side, val)  ( (*node)->side->node_elem.type == NUM && \
                             IsDoubleNumsEqual ((*node)->side->node_elem.elem.num, 0) )

	switch(node_oper)
	{
	case MUL:
		// CHECK MULTIPLYING SOMETHING ON 0
		if (IS (right, 0) || IS (left, 0)) 
		{
			DestroyNode(expr_tree, *node);
			*node = CREATE_NUM(0);
			*is_node_changed = true;
		}

		// CHECK MULTIPLYING SOMETHING ON 1 ON RIGHT
		DESTROY_UNNECESSARY_NODE_IF_NEEDED(node_left, node_right, 1);
		
		// CHECK MULTIPLYING SOMETHING ON 1 ON LEFT
		DESTROY_UNNECESSARY_NODE_IF_NEEDED(node_right, node_left, 1);

		break;

	case ADD:
		// CHECK MULTIPLYING SOMETHING TO 0 ON RIGHT
		DESTROY_UNNECESSARY_NODE_IF_NEEDED(node_left, node_right, 0);
		
		// CHECK MULTIPLYING SOMETHING TO 0 ON LEFT
		DESTROY_UNNECESSARY_NODE_IF_NEEDED(node_right, node_left, 0);

		break;

	case POW:
		// CHECKING POWING 1 AT ANY POW
		if (IS(left, 1)) 
		{
			DestroyNode(expr_tree, *node);
			*node = CREATE_NUM(1);
			*is_node_changed = true;
		}

		// CHECKING POWING SOMETHING INTO 0 POW
		if (IS(right, 0))
		{
			DestroyNode(expr_tree, *node);
			*node = CREATE_NUM(1);
			*is_node_changed = true;
		}

		if(node_left)
			DESTROY_UNNECESSARY_NODE_IF_NEEDED(node_left, node_right, 1);

		break;
	}

	return;
}

#undef DESTROY_UNNECESSARY_NODE_IF_NEEDED

void EvalConstants(Tree* expr_tree, TreeNode** node)
{
	assert(expr_tree != nullptr);
	assert(node      != nullptr);

	TreeNode* node_to_destroy = nullptr;
	
	node_to_destroy = *node;
	*node = CREATE_NUM(Eval(*node));
	DestroyNode(expr_tree, node_to_destroy);
}

void OptimizeNode(Tree* expr_tree, TreeNode** node, bool* is_node_changed)
{
	assert(expr_tree       != nullptr);
	assert(node            != nullptr);
	assert(is_node_changed != nullptr);

	if (!(*node))
		return;

	if ((*node)->node_elem.type == OPER)
		DeleteUnnecessaryNodeExpr(expr_tree, node, is_node_changed);

	if ((*node)->left && (*node)->right && ContainsNumsOnly((*node)))
	{
		EvalConstants(expr_tree, node);
		*is_node_changed = true;
	}

	OptimizeNode(expr_tree, &(*node)->right, is_node_changed);	
	OptimizeNode(expr_tree, &(*node)->left,  is_node_changed);
}

void OptimizeTree(Tree* expr_tree)
{
	assert(expr_tree != nullptr);

	bool is_node_changed = false;
	
	DO_LATEX_PRINTF(" \\newline \\newline ");
	DO_LATEX_PRINTF("Optimization bailnado: \\newline ");
	do
	{
		is_node_changed = false;

		OptimizeNode(expr_tree, &expr_tree->root, &is_node_changed);

		DoIntermidiatePrint(expr_tree, expr_tree->root);
		DO_LATEX_PRINTF(" \\newline ");
	} while (is_node_changed);
}
