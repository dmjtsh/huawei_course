#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <string.h>

#include "DimasLIB/DimasTree/tree.h"
#include "DimasLIB/DimasTextInfo/text_info.h"
#include "read_node.h"

void SetNum(TreeNode_t* data, double num)
{
	assert(data != nullptr);

	data->type     = NUM;
	data->elem.num = num;
}

void SetVar(TreeNode_t* data, char var)
{
	assert(data != nullptr);

	data->type          = VAR;
	data->elem.var.name = var;
}

bool TrySetOper(TreeNode_t* data, char* oper_str)
{
	assert(data     != nullptr);
	assert(oper_str != nullptr);

	bool is_oper_found = false;
	
	if(0);
	#define OPER_DEF(value, designation, ...)                      \
	else if(!is_oper_found && strcmp(designation, oper_str) == 0)  \
	{                                                              \
		is_oper_found = true;			                           \
		data->elem.oper = value;                                   \
		data->type = OPER;                                         \
	}
	
	#include "opers_defs.h"

	#undef OPER_DEF

	return is_oper_found;
}

bool TrySetStrElem(TreeNode_t* data, char* str, size_t str_len)
{
	assert(data != nullptr);
	assert(str  != nullptr);


	if (TrySetOper(data, str))
		return true;
	else if( str_len == 1)
	{
		SetVar(data, *str);
		return true;
	}
	else
		return false;
}

#define SET_ERROR_AND_RET_IF_NEEDED(cond, error)     \
	do                                               \
	{												 \
		if(!(cond))									 \
		{											 \
			*errors_ptr |= error;					 \
			return 0;								 \
		}											 \
	} while(0)

#define DO_SSCANF(scanf_format, var_to_write_in)																   \
	do																											   \
	{																											   \
		read_args_num = sscanf(tree_text_repr + read_ch_count, scanf_format "%n", var_to_write_in, &curr_str_len); \
		read_ch_count += curr_str_len + 1;                                                                         \
	} while(0)

#define DO_AND_CHECK_SSCANF(scanf_format, var_to_write_in, check_cond)                                          \
	do																										    \
	{																										    \
		DO_SSCANF(scanf_format, var_to_write_in);																\
		SET_ERROR_AND_RET_IF_NEEDED(check_cond, SOURCE_INVALID_SYNTAX);                                         \
	} while(0)


size_t ReadNode(Tree* expr_tree, TreeNode** node_ptr, const char* tree_text_repr, unsigned* errors_ptr)
{
	assert(node_ptr       != nullptr);
	assert(tree_text_repr != nullptr);
	assert(errors_ptr     != nullptr);         

	if(*errors_ptr) return 0;       

	size_t read_ch_count = 0;
	size_t curr_str_len  = 0;
	size_t read_args_num = 0;

	char       str_node_data[MAX_OPER_LEN] = "";
	double     num_node_data               = 0;
	TreeNode_t node_data = {};
	
	// OPEN BRACKET CHECK
	DO_AND_CHECK_SSCANF("%s", str_node_data, read_args_num == 1);

	if(str_node_data[0] == '(' && curr_str_len == 1)
	{
		// LEFT CHILD
		TreeNode* left_child = {};
		read_ch_count += ReadNode(expr_tree, &left_child, tree_text_repr + read_ch_count, errors_ptr);

		DO_SSCANF("%lf", &num_node_data);
		if(read_args_num != 1)
		{
			read_ch_count -= curr_str_len + 1; // GOING BACK AFTER PREVIOS SSCANF 

			DO_AND_CHECK_SSCANF("%s", str_node_data, read_args_num == 1 && TrySetStrElem(&node_data, str_node_data, curr_str_len));
		}
		else
			SetNum(&node_data, num_node_data);
		
		TreeNode* new_node = OpNew(expr_tree, &node_data);
		SET_ERROR_AND_RET_IF_NEEDED(new_node, NODE_CALLOC_ERROR);
		*node_ptr = new_node;

		// RIGHT CHILD
		TreeNode* right_child = {};
		read_ch_count += ReadNode(expr_tree, &right_child, tree_text_repr + read_ch_count, errors_ptr);
		
		(*node_ptr)->left  = left_child;
		(*node_ptr)->right = right_child;

		// CLOSE BRACKET CHECK
		DO_AND_CHECK_SSCANF("%s", str_node_data, read_args_num && str_node_data[0] == ')' && curr_str_len == 1);
	}
	else
		SET_ERROR_AND_RET_IF_NEEDED((strcmp(str_node_data, EMPTY_NODE) == 0), SOURCE_INVALID_SYNTAX);

	return read_ch_count;
}


unsigned InitializeExprTree(Tree* expr_tree, const char* source_file)
{
	assert(expr_tree != nullptr);

	TextInfo source_text_info = {};
	if(TextInfoCtor(&source_text_info, source_file))
		return TEXT_INFO_ERROR;
	
	unsigned errors = {};
	ReadNode(expr_tree, &expr_tree->root, source_text_info.text, &errors);

	TextInfoDtor(&source_text_info);

	if (errors)
		return SOURCE_INVALID_SYNTAX;
}

#undef DO_AND_CHECK_SSCANF
#undef DO_SSCANF_AND_MOVE
#undef SET_ERROR_AND_RET_IF_NEEDED