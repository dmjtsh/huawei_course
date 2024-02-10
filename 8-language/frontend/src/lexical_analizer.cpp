#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../../common/DimasLIB/DimasUtilities/utilities.h"
#include "../../common/DimasLIB/DimasTree/tree.h"
#include "../../common/nametable.h"
#include "syntax_analizer.h"

char* GetOperDesignation(Operation oper)
{
	static char design[MAX_OPER_LEN] = "";

	if(0);
	#define OPER_DEF(value, designation, ...) \
	else if(oper == value)                    \
		strcpy(design, designation);

	#include "../../common/opers_defs.h"

	#undef OPER_DEF

	return design;
}

Operation GetOperValue(const char* design)
{
	assert(design != nullptr);

	Operation oper = WRONG_OPER;

	if(0);
	#define OPER_DEF(value, designation, ...) \
	else if(strcmp(design, designation) == 0) \
		oper = value;
	
	#include "../../common/opers_defs.h"

	#undef OPER_DEF
		
	return oper;
}

bool IsOper(const char* design)
{
	assert(design != nullptr);

	bool is_oper_found = false;

	if(0);
	#define OPER_DEF(value, designation, ...)  \
	else if (strcmp(design, designation) == 0) \
		is_oper_found = true;

	#include "../../common/opers_defs.h"

	#undef OPER_DEF

	return is_oper_found;
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

TreeNode* CreateOperNode(Tree* expr_tree, Operation oper, TreeNode* left_node, TreeNode* right_node)
{
	assert(expr_tree != nullptr);;

	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type      = OPER;
	new_data.elem.oper = oper;

	return CreateNode(expr_tree, &new_data, &new_node, left_node, right_node);
}

TreeNode* CreateIdNode(Tree* expr_tree, NameTableElem* nametable_elem_ptr)
{
	assert(expr_tree != nullptr);

	TreeNode*  new_node = {};
	TreeNode_t new_data = {};

	new_data.type    = ID;
	new_data.elem.id = nametable_elem_ptr;

	return CreateNode(expr_tree, &new_data, &new_node, nullptr, nullptr);
}

size_t TrySetNum(Tree* expr_tree, TreeNode** token_ptr, char* code)
{
	assert(expr_tree != nullptr);
	assert(code      != nullptr);

	size_t curr_ch_num                  = 0;
	double num                          = 0;
	bool   int_part_of_num              = true;
	size_t fraction_part_digits_counter = 10;

	while (isdigit(code[curr_ch_num]) || code[curr_ch_num] == '.')
	{
		if(code[curr_ch_num] == '.')
			int_part_of_num = false;
		else
		{
			if(int_part_of_num)
				num = num * 10 + (code[curr_ch_num] - '0');
			else
			{
				num = num + (double)(code[curr_ch_num] - '0') / (fraction_part_digits_counter);
				
				fraction_part_digits_counter *= 10;
			}
		}

		curr_ch_num++;
	}

	// Num must contain smth
	if(!curr_ch_num)
		return 0;

	*token_ptr = CreateNumNode(expr_tree, num);

	return curr_ch_num;
}

// OPER SYMB ex: +, -, /, *, etc
size_t TrySetOperSymb(Tree* expr_tree, TreeNode** token_ptr, const char* code)
{
	assert(expr_tree != nullptr);
	assert(token_ptr != nullptr);
	assert(code      != nullptr);

	char oper_symb[MAX_ID_SIZE] = "";
	oper_symb[0] = code[0];

	if(IsOper(oper_symb))
	{
		*token_ptr = CreateOperNode(expr_tree, GetOperValue(oper_symb), nullptr, nullptr);

		return 1; // MOVING CODE STR ON 1
	}
	else
		return 0;
}

size_t SetIdStr(const char* code, char* id_str)
{
	assert(code   != nullptr);
	assert(id_str != nullptr);

	size_t curr_ch_num = 0;
	
	// First letter must be letter
	if(!IsCharLetter(code[curr_ch_num])) 
		return 0;

	while (IsCharLetter(code[curr_ch_num]) || isdigit(code[curr_ch_num]))
	{
		id_str[curr_ch_num] = code[curr_ch_num];

		curr_ch_num++;
	}

	return curr_ch_num;
}

size_t TrySetId(Tree* expr_tree, TreeNode** token_ptr, const char* code, NameTable* common_nametable, size_t* scopes_counter)
{
	assert(expr_tree        != nullptr);
	assert(token_ptr        != nullptr);
	assert(code             != nullptr);
	assert(common_nametable != nullptr);
	assert(scopes_counter   != nullptr);

	char   id_str[MAX_ID_SIZE] = "";
	size_t curr_ch_num         = 0;

	curr_ch_num += SetIdStr(code + curr_ch_num, id_str);

	// Id name must contain smth
	if(!curr_ch_num)
		return 0;

	if(IsOper(id_str))
	{																																							
		*token_ptr = CreateOperNode(expr_tree, GetOperValue(id_str), nullptr, nullptr);

		if(GetOperValue(id_str) == FUNC)
			(*scopes_counter)++;
	}
	else
	{
		NameTableElem* nametable_elem_ptr = NameTableFind(common_nametable, id_str);
		if(!nametable_elem_ptr)
			nametable_elem_ptr = NameTableAdd(common_nametable, id_str, common_nametable->size);
		
		*token_ptr = CreateIdNode(expr_tree, nametable_elem_ptr);
	}
	return curr_ch_num;
}

#define TRY_SET_ELEM_AND_MOVE_STR_P(try_set_func, ...)												   \
	do																								   \
	{																								   \
		old_ch_num = curr_ch_num;																	   \
		curr_ch_num += try_set_func(expr_tree, &tokens[token_count], code + curr_ch_num, __VA_ARGS__); \
		if(old_ch_num != curr_ch_num)																   \
			token_count++;																			   \
	} while(0)

TreeNode** DoLexicalAnalisys(Tree* expr_tree, const char* file_name, NameTable* common_nametable, size_t* scopes_counter)
{
	assert(expr_tree        != nullptr);
	assert(file_name        != nullptr);
	assert(common_nametable != nullptr);
	
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
		TRY_SET_ELEM_AND_MOVE_STR_P(TrySetId, common_nametable, scopes_counter);
	}

	TreeNode** tmp_tokens = (TreeNode**)realloc(tokens, (token_count+1) * sizeof(TreeNode*));
	if(!tmp_tokens)
		return nullptr;

	tokens = tmp_tokens;

	free(code);

	return tokens;
}

#undef DO_FUNC_AND_MOVE_STR_P