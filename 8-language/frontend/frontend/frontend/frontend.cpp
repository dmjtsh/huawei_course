#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "DimasLIB/DimasUtilities/utilities.h"
#include "DimasLIB/DimasTree/tree.h"
#include "lexical_analyzer.h"

#define TOKEN_TYPE tokens[*current_token_num]->node_elem.type
#define TOKEN_OPER tokens[*current_token_num]->node_elem.elem.oper

void NameTableAdd(NameTable* name_table, const char* designation, TreeElemType elem_type, int elem_code)
{
	assert(name_table  != nullptr);
	assert(designation != nullptr);

	int curr_elem_index = name_table->size;
	
	name_table->elems[curr_elem_index].str  = designation;
	name_table->elems[curr_elem_index].type = elem_type;
	name_table->elems[curr_elem_index].code = elem_code;
	
	name_table->size++;
}

void syn_assert(bool cond)
{
	if(!cond)
	{
		printf("pizdec");
		abort();
	}
}

TreeNode* GetExpr(TreeNode** tokens, size_t* current_token_num);
TreeNode* GetOper(TreeNode** tokens, size_t* current_token_num);

TreeNode* GetN(TreeNode** tokens, size_t* current_token_num)
{ 
	if(TOKEN_TYPE == NUM)
	{
		(*current_token_num)++;
		return tokens[*current_token_num-1];
	}

	return nullptr;
}

TreeNode* GetId(TreeNode** tokens,  size_t* current_token_num)
{
	if(tokens[*current_token_num]->node_elem.type == VAR)
	{
		(*current_token_num)++;
		return tokens[*current_token_num-1];
	}

	return nullptr;
}

TreeNode* GetPrimary(TreeNode** tokens, size_t* current_token_num)
{
	if (TOKEN_TYPE == OPER && TOKEN_OPER == OBR)
	{
		(*current_token_num)++;
		TreeNode* expr_token = GetExpr(tokens, current_token_num);
		
		syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
		(*current_token_num)++;
		
		syn_assert(expr_token != nullptr);
		return expr_token;
	}
	else if (TOKEN_TYPE == NUM)
	{
		TreeNode* num_token = GetN(tokens, current_token_num);
	
		syn_assert(num_token != nullptr);
		return num_token;
	}
	else if (TOKEN_TYPE == VAR)
	{
		TreeNode* id_token = GetId(tokens, current_token_num);

		syn_assert(id_token != nullptr);
		return id_token;
	}
	else
		syn_assert(0);
}

TreeNode* GetTerm(TreeNode** tokens, size_t* current_token_num)
{
	TreeNode* primary_node1 = GetPrimary(tokens, current_token_num);
	
	while(TOKEN_OPER == MUL || TOKEN_OPER == DIV)
	{
		size_t oper_token_num = *current_token_num;

		(*current_token_num)++;

		TreeNode* primary_node2 = GetPrimary(tokens, current_token_num);
		
		tokens[oper_token_num]->left  = primary_node1;
		tokens[oper_token_num]->right = primary_node2;	

		primary_node1 = tokens[oper_token_num];
	}

	return primary_node1;
}

TreeNode* GetExpr(TreeNode** tokens, size_t* current_token_num)
{
	TreeNode* term_node1 = GetTerm(tokens, current_token_num);
	
	while(TOKEN_OPER == ADD || TOKEN_OPER == SUB)
	{
		size_t oper_token_num = *current_token_num;

		(*current_token_num)++;

		TreeNode* term_node2 = GetTerm(tokens, current_token_num);
		
		tokens[oper_token_num]->left  = term_node1;
		tokens[oper_token_num]->right = term_node2;	

		term_node1 = tokens[oper_token_num];
	}

	return term_node1;
}

// NAMETABLE ÄÎÁÀÂÈÒÜ È ÀÑÑÅÐÒÛ È ÄÅÔÀÉÍÛ!!!!!!!!!!!!!!!!!!!!!!!!!!
TreeNode* GetAssign(TreeNode** tokens, size_t* current_token_num)
{
	TreeNode* id_node = GetId(tokens, current_token_num);
	if(!id_node)
		return nullptr;

	size_t assign_token_num = *current_token_num;
	
	tokens[assign_token_num]->left = id_node;
	
	syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == ASSIGN);
	(*current_token_num)++;

	tokens[assign_token_num]->right = GetExpr(tokens, current_token_num);

	return tokens[assign_token_num];
}

// ÂÎÒ ÒÓÒÀ ÄÅÔÀÉÍÛ
TreeNode* GetIf(TreeNode** tokens, size_t* current_token_num)
{
	if(!(TOKEN_TYPE == OPER && TOKEN_OPER == IF))
		return nullptr;

	size_t if_token_num = *current_token_num;

	(*current_token_num)++;
	
	syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
	(*current_token_num)++;
	
	TreeNode* expr_node = GetExpr(tokens, current_token_num);
	
	syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == CBR);
	(*current_token_num)++;

	TreeNode* oper_node = GetOper(tokens, current_token_num);

	syn_assert(oper_node != nullptr && expr_node != nullptr);

	tokens[if_token_num]->left  = expr_node;
	tokens[if_token_num]->right = oper_node;

	return tokens[if_token_num];
}

TreeNode* GetWhile(TreeNode** tokens, size_t* current_token_num)
{
	if(!(TOKEN_TYPE == OPER && TOKEN_OPER == WHILE))
		return nullptr;

	size_t while_token_num = *current_token_num;

	(*current_token_num)++;
	
	syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
	(*current_token_num)++;
	
	TreeNode* expr_node = GetExpr(tokens, current_token_num);
	
	syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == CBR);
	(*current_token_num)++;

	TreeNode* oper_node = GetOper(tokens, current_token_num);

	syn_assert(oper_node != nullptr && expr_node != nullptr);

	tokens[while_token_num]->left  = expr_node;
	tokens[while_token_num]->right = oper_node;

	return tokens[while_token_num];
}

TreeNode* GetOper(TreeNode** tokens, size_t* current_token_num)
{
	if(!tokens[*current_token_num])
		return nullptr;

	TreeNode* if_node     = GetIf(tokens, current_token_num);
	if(if_node)
		return if_node;

	TreeNode* while_node  = GetWhile(tokens, current_token_num);
	if(while_node)
		return while_node;

	TreeNode* assign_node = GetAssign(tokens, current_token_num);
	if(assign_node)
	{
		syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);
		return assign_node;
	}

	if (TOKEN_TYPE == OPER && TOKEN_OPER == OFBR)
	{
		TreeNode* oper_node1 = GetOper(tokens, current_token_num);
		TreeNode* oper_node2 = nullptr;
		syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);

		while(oper_node2 = GetOper(tokens, current_token_num))
		{
			size_t sep_token_num = *current_token_num;
			syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);

			tokens[sep_token_num]->left  = oper_node1;
			tokens[sep_token_num]->right = oper_node2;

			oper_node1 = tokens[sep_token_num];

			(*current_token_num)++;
		}
	
		syn_assert(TOKEN_OPER == CFBR);
		(*current_token_num)++;

		return oper_node1;
	}

	return nullptr;
}

TreeNode* GetExpr(TreeNode** tokens, size_t* current_token_num)
{
	TreeNode* term_node1 = GetTerm(tokens, current_token_num);
	
	while(TOKEN_OPER == ADD || TOKEN_OPER == SUB)
	{
		size_t oper_token_num = *current_token_num;

		(*current_token_num)++;

		TreeNode* term_node2 = GetTerm(tokens, current_token_num);
		
		tokens[oper_token_num]->left  = term_node1;
		tokens[oper_token_num]->right = term_node2;	

		term_node1 = tokens[oper_token_num];
	}

	return term_node1;
}

TreeNode* GetProgram(TreeNode** tokens, size_t* current_token_num)
{
	TreeNode* oper_node1 = GetOper(tokens, current_token_num);
	TreeNode* oper_node2 = nullptr;
	syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);

	while(oper_node2 = GetOper(tokens, current_token_num))
	{
		size_t sep_token_num = *current_token_num;
		syn_assert(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);

		tokens[sep_token_num]->left  = oper_node1;
		tokens[sep_token_num]->right = oper_node2;

		oper_node1 = tokens[sep_token_num];

		(*current_token_num)++;
	} 

	return oper_node1;
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

TreeNode* CreateVarNode(Tree* expr_tree, char* var_name)
{
	assert(expr_tree != nullptr);

	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type          = VAR;
	strcpy(new_data.elem.var.name, var_name);

	return CreateNode(expr_tree, &new_data, &new_node, nullptr, nullptr);
}

NameTableElem* NameTableFind(NameTable* name_table, const char* elem_to_find)
{
	for(size_t i = 0; i < name_table->size; i++)
	{
		if(strcmp(name_table->elems[i].str, elem_to_find) == 0)
			return &name_table->elems[i];
	}

	return nullptr;
}

void NameTableCtor(NameTable* name_table)
{
	assert(name_table != nullptr);

	#define OPER_DEF(code, designation, ...) NameTableAdd(name_table, designation, OPER, code);

	#include "opers_defs.h"

	#undef OPER_DEF
}

void NameTableDtor(NameTable* name_table)
{
	;
}

char* GetOperDesignation(Operation oper)
{
	static char design[MAX_OPER_LEN] = "";

	if(0);
	#define OPER_DEF(value, designation, ...) \
	else if(oper == value)                    \
		strcpy(design, designation);

	#include "opers_defs.h"

	#undef OPER_DEF

	return design;
}

char* ExprTreeElemPrinter(const TreeNode_t* elem_to_print)
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
	else if (elem_to_print->type == VAR)
		sprintf(str_to_output, "%s", elem_to_print->elem.var.name);
	else if (elem_to_print->type == OPER)
		sprintf(str_to_output, "%s", GetOperDesignation(elem_to_print->elem.oper));
	else
		assert(0);
	
	return str_to_output;
}

void ExprTreeElemCtor(TreeNode_t* new_elem, TreeNode_t* new_data)
{
	assert(new_elem != nullptr);
	assert(new_data != nullptr);

	*new_elem = *new_data;
} 

void ExprTreeElemDtor(TreeNode_t* elem_to_delete)
{
	assert(elem_to_delete != nullptr);
}

Tree GetCodeTree(const char* file_name)
{
	NameTable name_table = {}; 
	NameTableCtor(&name_table);
	
	TreeNode** tokens    = nullptr;
	Tree       expr_tree = {};
	TreeCtor(&expr_tree, ExprTreeElemCtor, ExprTreeElemDtor, ExprTreeElemPrinter);

	tokens = DoLexicalAnalisys(&expr_tree, file_name, &name_table);

	size_t current_token_num = 0;
	expr_tree.root = GetProgram(tokens, &current_token_num); 

	NameTableDtor(&name_table);

	return expr_tree;
}