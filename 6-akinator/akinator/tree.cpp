#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "tree.h"
#include "DimasLIB/DimasUtilities/error_processing.h"

TreeNode* OpNew(TreeNode_t data)
{
	assert(data != nullptr);

	TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
	if (!node) return nullptr;

	strcpy(node->elem, data);

	return node;
}

void OpDelete(TreeNode* node)
{
	assert(node != nullptr);

	free(node);
}

void PrintNode(const TreeNode* node, FILE* logger)
{
	if (!node) { fprintf(logger, EMPTY_NODE " "); return; }
	
	fprintf(logger, "( \"%s\" ", node->elem);
	
	PrintNode(node->left, logger);
	PrintNode(node->right, logger);
	
	fprintf(logger, ") ");
}

void DestroyNode(TreeNode** node_ptr)
{
	if (!node_ptr || !*node_ptr)
		return;

	if((*node_ptr)->left)
		DestroyNode(&(*node_ptr)->left);
	if((*node_ptr)->right)
		DestroyNode(&(*node_ptr)->right);

	free(*node_ptr);

	*node_ptr = 0;
}

size_t ReadNode(TreeNode** node_ptr, const char* tree_text_repr, unsigned* errors_ptr)
{
	assert(node_ptr       != nullptr);
	assert(tree_text_repr != nullptr);
	assert(errors_ptr     != nullptr);

	#define DESTROY_TREE_AND_RET(node_ptr, error_bit) \
		do									          \
		{									          \
			*errors_ptr &= error_bit;                 \
			DestroyNode(node_ptr);                    \
			return 0;                                 \
		} while(0)

	#define DO_AND_CHECK_SSCANF(check_cond, scanf_format)                                                           \
		do																										    \
		{																										    \
			read_args_num = sscanf(tree_text_repr + read_ch_count, scanf_format "%n", node_data, &current_str_len); \
			read_ch_count += current_str_len + 1;                                                                   \
			if (!(check_cond))                                                                                      \
			{                                                                                                       \
				DESTROY_TREE_AND_RET(node_ptr, TREE_STR_INVALID);                                                   \
			}																										\
		} while(0)


	if(*errors_ptr)
		DESTROY_TREE_AND_RET(node_ptr, *errors_ptr);

	size_t read_ch_count     = 0;
	size_t current_str_len = 0;
	size_t read_args_num     = 0;
	char node_data[MAX_NODE_STR_LEN] = "";
                                                  
	DO_AND_CHECK_SSCANF(read_args_num == 1, "%s");

	if(node_data[0] == '(' && current_str_len == 1)
	{

		DO_AND_CHECK_SSCANF(read_args_num == 1, " \"%[^\"]\"");

		TreeNode* new_node = OpNew(node_data);
		if(!new_node)
			DESTROY_TREE_AND_RET(node_ptr, NODE_CALLOC_ERROR);

		*node_ptr = new_node;

		read_ch_count += ReadNode(&(*node_ptr)->left,  tree_text_repr + read_ch_count, errors_ptr);
		read_ch_count += ReadNode(&(*node_ptr)->right, tree_text_repr + read_ch_count, errors_ptr);
		
		DO_AND_CHECK_SSCANF(read_args_num && node_data[0] == ')' && current_str_len == 1, "%s");
	}
	else
	{
		if (!(strcmp(node_data, EMPTY_NODE) == 0))
			DESTROY_TREE_AND_RET(node_ptr, TREE_STR_INVALID);
	}

	return read_ch_count;

	#undef DESTROY_TREE_AND_RET
	#undef DO_AND_CHECK_SSCANF
}

void TreeDump(Tree* tree, FILE* logger)
{
	assert(tree   != nullptr);
	assert(logger != nullptr);

	PrintNode(tree->root, logger);
}

unsigned TreeVerifier(Tree* tree)
{
	if (!tree)
		return TREE_PTR_NULL;

	CHECK_ERROR(tree, !tree->root, ROOT_PTR_NULL);
}


unsigned TreeCtor(Tree* tree, char* tree_text_repr)
{
	assert(tree != nullptr);
	assert(tree_text_repr != nullptr);

	ReadNode(&tree->root, tree_text_repr, &tree->errors);
	if(!tree->root)
	{
		ReadNode(&tree->root, "( \"Noname\" nil nil )", &tree->errors);
		return TREE_STR_INVALID;
	}

	return 0;
}

unsigned TreeDtor(Tree* tree)
{
	assert(tree != nullptr);

	if (!(tree->errors & TREE_DELETED))
	{
		DestroyNode(&tree->root);
		tree->errors |= TREE_DELETED;
	}
	else
		return TREE_DELETED;

	return 0;
}