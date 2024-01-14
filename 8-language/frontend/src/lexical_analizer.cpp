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

TreeNode* CreateVarNode(Tree* expr_tree, const char* var_name)
{
	assert(expr_tree != nullptr);

	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type = VAR;
	strcpy(new_data.elem.var.name, var_name);

	return CreateNode(expr_tree, &new_data, &new_node, nullptr, nullptr);
}

size_t TrySetNum(Tree* expr_tree, TreeNode** token_ptr, char* code, NameTable* nametable)
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

size_t TrySetId(Tree* expr_tree, TreeNode** token_ptr, const char* code, NameTable* nametable)
{
	assert(expr_tree != nullptr);
	assert(token_ptr != nullptr);
	assert(code      != nullptr);

	size_t curr_ch_num           = 0;
	char   name_str[MAX_ID_SIZE] = "";

	// First letter must be letter
	if(!IsCharLetter(code[curr_ch_num])) 
		return 0;

	while (IsCharLetter(code[curr_ch_num]) || isdigit(code[curr_ch_num]))
	{
		name_str[curr_ch_num] = code[curr_ch_num];

		curr_ch_num++;
	}

	NameTableElem* nametable_elem_ptr = NameTableFind(nametable, name_str);

	if(IsKeyword(nametable, name_str))
	{
		assert(nametable_elem_ptr->type == KEYWORD);
		*token_ptr = CreateOperNode(expr_tree, (Operation)nametable_elem_ptr->code, nullptr, nullptr);
	}
	else if (!nametable_elem_ptr)
	{
		NameTableAdd(nametable, name_str, VARIABLE, nametable->size);
		*token_ptr = CreateVarNode(expr_tree, name_str);
	}
	else
		*token_ptr = CreateVarNode(expr_tree, name_str);		

	return curr_ch_num; 
}

// OPER SYMB ex: +, -, /, *, (, etc
size_t TrySetOperSymb(Tree* expr_tree, TreeNode** token_ptr, const char* code, NameTable* nametable)
{
	assert(expr_tree != nullptr);
	assert(token_ptr != nullptr);
	assert(code      != nullptr);

	char oper_symb[MAX_ID_SIZE] = "";
	oper_symb[0] = code[0];

	NameTableElem* keyword_ptr = NameTableFind(nametable, oper_symb);

	if(keyword_ptr)
	{
		*token_ptr = CreateOperNode(expr_tree, (Operation)keyword_ptr->code, nullptr, nullptr);
		return 1; // MOVING CODE STR ON 1
	}
	else
		return 0;
}

#define TRY_SET_ELEM_AND_MOVE_STR_P(try_set_func)													  \
	do																								  \
	{																								  \
		old_ch_num = curr_ch_num;																	  \
		curr_ch_num += try_set_func(expr_tree, &tokens[token_count], code + curr_ch_num, nametable); \
		if(old_ch_num != curr_ch_num)												                  \
			token_count++;																              \
	} while(0)

TreeNode** DoLexicalAnalisys(Tree* expr_tree, const char* file_name, NameTable* nametable)
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

	free(code);

	return tokens;
}

#undef DO_FUNC_AND_MOVE_STR_P