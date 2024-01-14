#define _CRT_SECURE_NO_WARNINGS

#include "DimasLIB/DimasTree/tree.h"
#include "ast_tree.h"
#include "nametable.h"
#include "../frontend/src/lexical_analizer.h"

#include <assert.h>
#include <string.h>

TreeNode* GetProperNode(Tree* tree, FILE* ast_file)
{
	double num_node_arg            = 0;
	char str_node_arg[MAX_ID_SIZE] = "";

	size_t read_args_num = 0;

	read_args_num = fscanf(ast_file, " %lf", &num_node_arg);
	if(!read_args_num)
	{
		read_args_num = fscanf(ast_file, " %s", str_node_arg);
		if(IsOper(str_node_arg))
			return CreateOperNode(tree, GetOperValue(str_node_arg), nullptr, nullptr);
		else
			return CreateVarNode(tree, str_node_arg);
	}
	else
		return CreateNumNode(tree, num_node_arg);
}

#define SET_ERROR_AND_RET_IF_NEEDED(cond, error) \
	if(!cond)									 \
	{											 \
		*errors |= error;					     \
		return;									 \
	}											 \

void ReadNode(Tree* tree, TreeNode** node_ptr, FILE* ast_file, unsigned* errors)
{
	assert(tree     != nullptr);
	assert(node_ptr != nullptr);
	assert(ast_file != nullptr);

	if(*errors)
		return;

	size_t read_args_num = 0;
	size_t cmd_len       = 0;
	
	read_args_num = fscanf(ast_file, " (%n", &cmd_len);
	if(!cmd_len)
	{
		char tmp_str[MAX_ID_SIZE] = "";
		read_args_num = fscanf(ast_file, " %s", tmp_str);			
		SET_ERROR_AND_RET_IF_NEEDED(strcmp(tmp_str, EMPTY_NODE) != 0, INVALID_AST_FORMAT);
	}

	*node_ptr = GetProperNode(tree, ast_file);

	ReadNode(tree, &(*node_ptr)->left, ast_file, errors);
	ReadNode(tree, &(*node_ptr)->right, ast_file, errors);
	
	read_args_num = fscanf(ast_file, " )%n", &cmd_len);
	SET_ERROR_AND_RET_IF_NEEDED(cmd_len, INVALID_AST_FORMAT);
}

void ReadTree(Tree* tree, FILE* ast_file, unsigned* errors)
{
	assert(tree     != nullptr);
	assert(ast_file != nullptr);

	size_t valid_args_num = 0;
	size_t cmd_len        = 0;

	valid_args_num = fscanf(ast_file, " Tree: {%n", &cmd_len);
	SET_ERROR_AND_RET_IF_NEEDED(cmd_len, INVALID_AST_FORMAT);

	ReadNode(tree, &tree->root, ast_file, errors);
	
	valid_args_num = fscanf(ast_file, " }%n", &cmd_len);
	SET_ERROR_AND_RET_IF_NEEDED(cmd_len, INVALID_AST_FORMAT);
}

const int REC_INDENT_SIZE = 2;

void WriteNodePreOrder(Tree* tree, const TreeNode* node, FILE* logger, int rec_depth)
{
	assert(tree   != nullptr);
	assert(logger != nullptr);
	
	if (!node) { fprintf(logger, "%*s" EMPTY_NODE "\n", rec_depth * REC_INDENT_SIZE, ""); return; }
	
	fprintf(logger, "%*s(", rec_depth * REC_INDENT_SIZE, "");

	fprintf(logger, "%s\n", tree->ElemPrinter(&node->node_elem));
	WriteNodePreOrder(tree, node->left,  logger, rec_depth + 1);
	WriteNodePreOrder(tree, node->right, logger, rec_depth + 1);
	
	fprintf(logger, "%*s)\n", rec_depth * REC_INDENT_SIZE, "");
}