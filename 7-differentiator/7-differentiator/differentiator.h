#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "DimasLIB/DimasTree/tree.h"
#include "DimasLIB/DimasTextInfo/text_info.h"
#include "DimasLIB/DimasUtilities/utilities.h"

const size_t MAX_COMMAND_LENGTH = 100;

#define NODE_TYPE         node->node_elem.type
#define NODE_NUM          node->node_elem.elem.num
#define NODE_OPER         node->node_elem.elem.oper
#define OPER_PRIORITY     node->node_elem.elem.oper.priority
#define NEW_DATA_OPER_NUM new_data.elem.oper.oper_num
#define VAR_NAME          node->node_elem.elem.var.name
#define VAR_VALUE         node->node_elem.elem.var.value

#define CREATE_OPER(operation, left, right) CreateOperNode(expr_tree, operation, left, right)
#define CREATE_NUM(number)                  CreateNumNode(expr_tree, number)
#define CREATE_VAR(var_name)                CreateVarNode(expr_tree, var_name) 

struct IntermediatePrinter
{
	TextInfo prints_text_info;
	size_t   current_str_num;

	FILE* latex_file;
};

double Eval(const TreeNode* node);

Tree GetDifferentiatedTree(Tree tree, char var);
Tree GetDifferentiatedNTimesTree(Tree tree, char var, size_t n);
Tree GetTaylorSerieTree(Tree expr_tree, char var, size_t serie_length, double point);

double EvalTreeInPoint(Tree* tree, double point);

TreeNode* CreateNumNode(Tree* expr_tree, double num);
TreeNode* CreateVarNode(Tree* expr_tree, char var_name);
TreeNode* CreateOperNode(Tree* expr_tree, Operation oper, TreeNode* left_node, TreeNode* right_node);

bool ContainsNumsOnly(TreeNode* node); 

void DifferentiatorElemCtor(TreeNode_t* new_elem, TreeNode_t* new_data);
void DifferentiatorElemDtor(TreeNode_t* elem_to_delete);

#endif DIFFERENTIATOR_H // DIFFERENTIATOR_H