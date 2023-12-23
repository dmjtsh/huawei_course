#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "DimasLIB/DimasTextInfo/text_info.h"
#include "DimasLIB/DimasUtilities/utilities.h"
#include "DimasLIB/DimasTree/tree.h"
#include "frontend.h"

size_t TrySetNum(Tree* expr_tree, TreeNode** token_ptr, char* code, NameTable* name_table)
{
	assert(expr_tree != nullptr);
	assert(code      != nullptr);

	size_t curr_ch_num = 0;
	int    num         = 0;

	while (isdigit(code[curr_ch_num]))
	{
		num = num * 10 + (code[curr_ch_num] - '0'); // TODO: добавить счет папича

		curr_ch_num++;
	}

	*token_ptr = CreateNumNode(expr_tree, num);

	return curr_ch_num;
}

size_t TrySetId(Tree* expr_tree, TreeNode** token_ptr, const char* code, NameTable* name_table)
{
	assert(expr_tree != nullptr);
	assert(token_ptr != nullptr);
	assert(code      != nullptr);

	size_t curr_ch_num           = 0;
	char   name_str[MAX_ID_SIZE] = "";

	// First letter must be letter
	if(!isalpha(code[curr_ch_num])) 
		return 0;

	while (isalnum(code[curr_ch_num]))
	{
		name_str[curr_ch_num] = code[curr_ch_num];

		curr_ch_num++;
	}

	NameTableElem* keyword_ptr = NameTableFind(name_table, name_str);

	if(keyword_ptr)
	{
		assert(keyword_ptr->type == OPER);
		*token_ptr = CreateOperNode(expr_tree, (Operation)keyword_ptr->code, nullptr, nullptr);
	}
	else
		*token_ptr = CreateVarNode(expr_tree, name_str);

	return curr_ch_num; 
}

// OPER SYMB ex: +, -, /, *, (, etc
size_t TrySetOperSymb(Tree* expr_tree, TreeNode** token_ptr, const char* code, NameTable* name_table)
{
	assert(expr_tree != nullptr);
	assert(token_ptr != nullptr);
	assert(code      != nullptr);

	char oper_symb[MAX_ID_SIZE] = "";
	oper_symb[0] = code[0];

	NameTableElem* keyword_ptr = NameTableFind(name_table, oper_symb);

	if(keyword_ptr)
		return 1; // MOVING CODE STR ON 1
	else
		return 0;
}

#define TRY_SET_ELEM_AND_MOVE_STR_P(try_set_func)													  \
	do																								  \
	{																								  \
		old_ch_num = curr_ch_num;																	  \
		curr_ch_num += try_set_func(expr_tree, &tokens[token_count], code + curr_ch_num, name_table); \
		if(old_ch_num != curr_ch_num)												                  \
			token_count++;																              \
	} while(0)

// TODO: добавить реаллок сюды
TreeNode** DoLexicalAnalisys(Tree* expr_tree, const char* file_name, NameTable* name_table)
{
	assert(expr_tree != nullptr);
	assert(file_name != nullptr);
	
	size_t file_size   = GetFileSize(file_name);
	TreeNode** tokens  = (TreeNode**)calloc(file_size, sizeof(TreeNode*));
	assert(tokens != nullptr);
	char* code         = GetFileText(file_name);

	size_t curr_ch_num = 0;
	size_t old_ch_num  = 0;
	size_t token_count = 0;

	while (curr_ch_num < file_size)
	{
		if(code[curr_ch_num] == '\0')
			break;

		if (isspace(code[curr_ch_num])) 
		{
			curr_ch_num++;
			continue;
		}

		TRY_SET_ELEM_AND_MOVE_STR_P(TrySetNum);
		TRY_SET_ELEM_AND_MOVE_STR_P(TrySetOperSymb);
		TRY_SET_ELEM_AND_MOVE_STR_P(TrySetId);
	}

	tokens = (TreeNode**)realloc(tokens, (token_count+1) * sizeof(TreeNode*));
	assert(tokens != nullptr);

	return tokens;
}

#undef DO_FUNC_AND_MOVE_STR_P