#define _CRT_SECURE_NO_WARNINGS

#include "DimasLIB/DimasTree/tree.h"
#include "DimasLIB/DimasUtilities/utilities.h"
#include "ast_tree.h"
#include "nametable.h"
#include "program_nametables.h"
#include "../frontend/src/lexical_analizer.h"
#include "../frontend/src/syntax_analizer.h"

#include <assert.h>
#include <string.h>

TreeNode* GetProperNode(Tree* tree, ProgramNameTables* nametables, int* current_scope, FILE* ast_file)
{
	assert(tree          != nullptr);
	assert(nametables    != nullptr);
	assert(current_scope != nullptr);
	assert(ast_file      != nullptr);

	double num_node_arg            = 0;
	char str_node_arg[MAX_ID_SIZE] = "";
	size_t file_cursor_pos         = 0;

	size_t read_args_num = 0;	
	
	file_cursor_pos = ftell(ast_file);
	read_args_num   = fscanf(ast_file, " %lf", &num_node_arg);
	
	if(!read_args_num)
	{	
		fseek(ast_file, file_cursor_pos, SEEK_SET);

		read_args_num = fscanf(ast_file, " %s", str_node_arg);
		
		if(!read_args_num)
			return nullptr;

		if(IsOper(str_node_arg))
		{
			if(GetOperValue(str_node_arg) == FUNC)
				(*current_scope)++;

			return CreateOperNode(tree, GetOperValue(str_node_arg), nullptr, nullptr);
		}
		else
		{
			TreeNode*         id_node = nullptr;
			NameTableElemType id_type = WRONG_NT_TYPE;
			read_args_num = fscanf(ast_file, " type:%d", &id_type);

			if(!read_args_num)
				return nullptr;

			if(id_type == VARIABLE)
				id_node = CreateIdNode(tree, NameTableFind(&nametables->scopes_nametables[*current_scope], str_node_arg));			
			else if(id_type == FUNCTION)
				id_node = CreateIdNode(tree, NameTableFind(&nametables->functions_nametable, str_node_arg));
			else
				assert(0);

			id_node->node_elem.elem.id->type = id_type;
			
			return id_node;
		}
	}
	else
		return CreateNumNode(tree, num_node_arg);
}

#define SET_ERROR_AND_RET_IF_NEEDED(cond, error) \
	if(!(cond))									 \
	{											 \
		*errors |= error;					     \
		return;									 \
	}											 \

void ReadNode(Tree* tree, TreeNode** node_ptr, ProgramNameTables* nametables, int* current_scope, FILE* ast_file, unsigned* errors)
{
	assert(tree          != nullptr);
	assert(node_ptr      != nullptr);
	assert(ast_file      != nullptr);
	assert(current_scope != nullptr);
	assert(errors        != nullptr);

	if(*errors)
		return;

	size_t read_args_num = 0;
	int cmd_len          = 0;
	
	read_args_num = fscanf(ast_file, " (%n", &cmd_len);

	if(!cmd_len)
	{
		char tmp_str[MAX_ID_SIZE] = "";
		read_args_num = fscanf(ast_file, " %s", tmp_str);			
		SET_ERROR_AND_RET_IF_NEEDED(strcmp(tmp_str, EMPTY_NODE) == 0, INVALID_AST_FORMAT);

		return;
	}

	*node_ptr = GetProperNode(tree, nametables, current_scope, ast_file);
	SET_ERROR_AND_RET_IF_NEEDED(*node_ptr, INVALID_AST_FORMAT);

	ReadNode(tree, &(*node_ptr)->left,  nametables, current_scope, ast_file, errors);
	ReadNode(tree, &(*node_ptr)->right, nametables, current_scope, ast_file, errors);

	read_args_num = fscanf(ast_file, " )%n", &cmd_len);
	SET_ERROR_AND_RET_IF_NEEDED(cmd_len, INVALID_AST_FORMAT);
}

void ReadASTTree(Tree* tree, ProgramNameTables* nametables, FILE* ast_file, unsigned* errors)
{
	assert(tree     != nullptr);
	assert(ast_file != nullptr);

	size_t valid_args_num = 0;
	int cmd_len           = 0;

	valid_args_num = fscanf(ast_file, " Tree: {%n", &cmd_len);
	SET_ERROR_AND_RET_IF_NEEDED(cmd_len, INVALID_AST_FORMAT);

	// BEGGING SCOPE (NO FUNC ENCOUNTERED)
	int current_scope = -1;
	ReadNode(tree, &tree->root, nametables, &current_scope, ast_file, errors);
	
	valid_args_num = fscanf(ast_file, " }%n", &cmd_len);
	SET_ERROR_AND_RET_IF_NEEDED(cmd_len, INVALID_AST_FORMAT);
}


const int REC_INDENT_SIZE = 2;

void WriteNodePreOrder(Tree* tree, const TreeNode* node, FILE* ast_file, int rec_depth)
{
	assert(tree     != nullptr);
	assert(node     != nullptr);
	assert(ast_file != nullptr);
	
	if (!node) { fprintf(ast_file, "%*s" EMPTY_NODE "\n", rec_depth * REC_INDENT_SIZE, ""); return; }
	
	fprintf(ast_file, "%*s(", rec_depth * REC_INDENT_SIZE, "");

	fprintf(ast_file, "%s\n", tree->ElemPrinter(&node->node_elem));
	WriteNodePreOrder(tree, node->left,  ast_file, rec_depth + 1);
	WriteNodePreOrder(tree, node->right, ast_file, rec_depth + 1);
	
	fprintf(ast_file, "%*s)\n", rec_depth * REC_INDENT_SIZE, "");
}

void WriteASTTree(Tree* tree, FILE* logger)
{
	assert(tree   != nullptr);
	assert(logger != nullptr);

	fprintf(logger, "Tree: {\n");
	WriteNodePreOrder(tree, tree->root, logger, 1);
	fprintf(logger, "}");
}

char* ASTTreeElemPrinter(const TreeNode_t* elem_to_print)
{
	assert(elem_to_print != nullptr);

	static char str_to_output[MAX_ID_SIZE] = "";

	if (elem_to_print->type == NUM)
	{
		if (IsDoubleInt(elem_to_print->elem.num))
			sprintf(str_to_output, "%d", (int)elem_to_print->elem.num);
		else
			sprintf(str_to_output, "%lf", elem_to_print->elem.num);
	}
	else if (elem_to_print->type == ID)
	{
		if(!elem_to_print->elem.id)
			sprintf(str_to_output, "%s", "Elem Var PIZDEC NULL");
		else
			sprintf(str_to_output, "%s type:%d", elem_to_print->elem.id->str, elem_to_print->elem.id->type);
	}
	else if (elem_to_print->type == OPER)
		sprintf(str_to_output, "%s", GetOperDesignation(elem_to_print->elem.oper));
	else
		assert(0);
	
	return str_to_output;
}

void ASTTreeElemCtor(TreeNode_t* new_elem, TreeNode_t* new_data)
{
	assert(new_elem != nullptr);
	assert(new_data != nullptr);

	*new_elem = *new_data;
} 

void ASTTreeElemDtor(TreeNode_t* elem_to_delete)
{
	assert(elem_to_delete != nullptr);
}