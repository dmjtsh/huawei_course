#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "differentiator.h"
#include "operation.h"
#include "read_node.h"
#include "differentiator_io.h"
#include "DimasLIB/DimasUtilities/error_processing.h"
#include "DimasLIB/DimasUtilities/utilities.h"

IntermediatePrinter INTERMEDIATE_PRINTER = {};

int Factorial(int n) 
{
    if (n == 0 || n == 1)
        return 1;
	else 
        return n * Factorial(n - 1);
}

double Eval(const TreeNode* node)
{
	if (!node)
		return 0;

	if (NODE_TYPE == NUM)
		return NODE_NUM;
	if (NODE_TYPE == VAR)
		return VAR_VALUE;

	double left  = Eval(node->left);
	double right = Eval(node->right);

	switch(NODE_OPER)
	{
		case ADD:
			return left + right;
		case SUB:
			return left - right;
		case MUL:
			return left * right;
		case DIV:
			return left / right;
		case POW:
			return pow(left, right);
		case LN:
			return log(right);
		case SIN:
			return sin(right);
		case COS:
			return cos(right);

		default:
			assert(0);
	}
}

void SetNodeVarValue(TreeNode* node, double value)
{
	if (!node || NODE_TYPE == NUM)
		return;

	if (NODE_TYPE == VAR)
		VAR_VALUE = value;

	SetNodeVarValue(node->left,  value);
	SetNodeVarValue(node->right, value);
}

double EvalTreeInPoint(Tree* expr_tree, double point)
{
	SetNodeVarValue(expr_tree->root, point);
	return Eval(expr_tree->root);
}

TreeNode* CreateOperNode(Tree* expr_tree, Operation oper, TreeNode* left_node, TreeNode* right_node)
{
	assert(expr_tree != nullptr);;

	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type      = OPER;
	new_data.elem.oper = oper;

	return CreateNode(expr_tree, &new_data, &new_node, left_node, right_node);
}

TreeNode* CreateVarNode(Tree* expr_tree, char var_name)
{
	assert(expr_tree != nullptr);

	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type          = VAR;
	new_data.elem.var.name = var_name;

	return CreateNode(expr_tree, &new_data, &new_node, nullptr, nullptr);
}

TreeNode* CreateNumNode(Tree* expr_tree, double num)
{
	assert(expr_tree != nullptr);

	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type     = NUM;       
	new_data.elem.num = num;

	return CreateNode(expr_tree, &new_data, &new_node, nullptr, nullptr);
}


bool ContainsNumsOnly(TreeNode* node)
{
	if (!node)
		return false;
	// TODO: ¬ÂËÙËÍ‡ÚÓ Ò˛‰˚

	if (NODE_TYPE == NUM && !node->left && !node->right)
		return true;

	if (NODE_TYPE == VAR)
		return false;

	if (ContainsNumsOnly(node->left) && ContainsNumsOnly(node->right))
		return true;

	return false;
}

// TODO: —ƒ≈À¿“‹ ¬ ¬≈–»‘» ¿“Œ–≈ œ–Œ¬≈– ” Õ¿ Œƒ»Õ ”«≈À
TreeNode* DifferentiateNode(Tree* expr_tree, TreeNode* node, char var)
{
	assert(expr_tree != nullptr);
	
	if(node == nullptr)
		return nullptr;
	
	TreeNode*  new_node = {};
	TreeNode_t new_data = {};

	DoIntermidiatePrint(expr_tree, node);

	if (NODE_TYPE == NUM)
	{
		new_node = CREATE_NUM(0);

		DoIntermidiatePrint(expr_tree, new_node);
		DO_LATEX_PRINTF(" \\newline ");

		return new_node;
	}

	if (NODE_TYPE == VAR)
	{
		new_node = CREATE_NUM(VAR_NAME == var);

		DoIntermidiatePrint(expr_tree, new_node);
		DO_LATEX_PRINTF(" \\newline ");

		return new_node;
	}

	if(NODE_OPER)
		DO_LATEX_PRINTF(" \\newline ");

	switch (NODE_OPER)
	{
		#define OPER_DEF(value, designation, priority, operation_num_args, latex_printing_code, differentiate_code, ...)  \
			case value:																									  \
					differentiate_code;																					  \
					break;
		
		#include "opers_defs.h"

		default: assert(0);
	}
	#undef OPER_DEF
	
	DoIntermidiatePrint(expr_tree, new_node);

	return new_node;
}

Tree GetDifferentiatedNTimesTree(Tree expr_tree, char var, size_t n)
{
	Tree result = expr_tree;

	for(size_t i = 0; i < n; i++)
	{
		TreeNode* old_node = result.root;  
		result.root = DifferentiateNode(&result, result.root, var);
		DestroyNode(&result, old_node);
	}

	return result;
}

Tree GetDifferentiatedTree(Tree expr_tree, char var)
{
	return GetDifferentiatedNTimesTree(expr_tree, var, 1);
}

Tree GetTaylorSerieTree(Tree expr_tree, char var, size_t serie_length, double point)
{
	Tree result_tree            = expr_tree;
	Tree diffed_tree            = {};
	TreeCtor(&diffed_tree, DifferentiatorElemCtor, DifferentiatorElemDtor, DifferentiatorElemPrinter);

	TreeNode* exp_node          = nullptr;
	TreeNode* previous_exp_node = nullptr;
	double tree_val_in_point    = 0;
	size_t factorial            = 1;

	for(size_t i = 0; i < serie_length; i++)
	{
		diffed_tree        = GetDifferentiatedNTimesTree(expr_tree, var, i);
		tree_val_in_point  = EvalTreeInPoint(&diffed_tree, point);
		DestroyNode(&diffed_tree, diffed_tree.root);

		factorial = Factorial(i);
		
		exp_node  = CreateOperNode(&expr_tree, SUB, 
									CreateVarNode(&diffed_tree, 'x'), CreateNumNode(&diffed_tree, point));

		exp_node  = CreateOperNode(&expr_tree, POW,
									CreateVarNode(&diffed_tree, 'x'), CreateNumNode(&diffed_tree, i));

		exp_node  = CreateOperNode(&expr_tree, DIV,
									exp_node, CreateNumNode(&diffed_tree, factorial));
		
		exp_node  = CreateOperNode(&expr_tree, MUL,
									exp_node, CreateNumNode(&diffed_tree, tree_val_in_point));

		if (i != 0)
			exp_node = CreateOperNode(&expr_tree, ADD,
										exp_node, previous_exp_node);

		previous_exp_node = exp_node;
	}
		
	result_tree.root = exp_node;

	TreeDtor(&diffed_tree);

	return result_tree;
}

void DifferentiatorElemCtor(TreeNode_t* new_elem, TreeNode_t* new_data)
{
	assert(new_elem != nullptr);
	assert(new_data != nullptr);

	*new_elem = *new_data;
} 

void DifferentiatorElemDtor(TreeNode_t* elem_to_delete)
{
	assert(elem_to_delete != nullptr);
}
