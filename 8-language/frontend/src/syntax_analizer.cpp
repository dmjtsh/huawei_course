#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../../common/DimasLIB/DimasUtilities/utilities.h"
#include "../../common/DimasLIB/DimasTree/tree.h"
#include "../../common/ast_tree.h"
#include "lexical_analizer.h"

#define TOKEN_TYPE      tokens[*current_token_num]->node_elem.type
#define TOKEN_OPER      tokens[*current_token_num]->node_elem.elem.oper
#define TOKEN_VAR_NAME  tokens[*current_token_num]->node_elem.elem.var->str
#define TOKEN_NUM       tokens[*current_token_num]->node_elem.elem.num

 //---/\---/\-------Ýòî ASCII KOT!--//
 //  {  '-'  }                      //
 //  {  0 0  }     Äîáàâü åãî ñåáå  //
 //  --> V <--  â èñõîäíèê, è òåáå  //
 //   \ \|/ /      áóäåò, íàâåðíî,  //
 //    \___/  ïðèÿòíî îòëàæèâàòüñÿ  //
 //---------------äîëãèìè íî÷àìè:)--//

void PrintCurrentToken(TreeNode** tokens, size_t* current_token_num)
{
	assert(tokens);
	assert(current_token_num);

	switch(TOKEN_TYPE)
	{
		case VAR:
			printf("%s\n", TOKEN_VAR_NAME);
			break;
		case NUM:
			printf("%lf\n", TOKEN_NUM);
			break;
		case OPER:
			printf("%s\n", GetOperDesignation(TOKEN_OPER));
			break;
		default:
			assert(0);
	}
}

#define SYN_ASSERT(condition)						   \
	if(!(condition))								   \
	{												   \
		printf("Token TYPE: %d\n", TOKEN_TYPE);        \
		PrintCurrentToken(tokens, current_token_num);  \
		abort();									   \
	}

/****************************************************************************************************************
* 
* RECURSIVE DESCENT BLOCK START
* 
*****************************************************************************************************************/

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
		
		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
		(*current_token_num)++;
		
		SYN_ASSERT(expr_token != nullptr);
		return expr_token;
	}
	else if (TOKEN_TYPE == NUM)
	{
		TreeNode* num_token = GetN(tokens, current_token_num);
	
		SYN_ASSERT(num_token != nullptr);
		return num_token;
	}
	else if (TOKEN_TYPE == VAR)
	{
		TreeNode* id_token = GetId(tokens, current_token_num);

		SYN_ASSERT(id_token != nullptr);
		return id_token;
	}
	else
		SYN_ASSERT(0);
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
	
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == ASSIGN);
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
	
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
	(*current_token_num)++;
	
	TreeNode* expr_node = GetExpr(tokens, current_token_num);
	
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == CBR);
	(*current_token_num)++;

	TreeNode* oper_node = GetOper(tokens, current_token_num);

	SYN_ASSERT(oper_node != nullptr && expr_node != nullptr);

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
	tokens[*current_token_num-1];
	tokens[*current_token_num];

	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
	(*current_token_num)++;
	
	TreeNode* expr_node = GetExpr(tokens, current_token_num);
	
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == CBR);
	(*current_token_num)++;

	TreeNode* oper_node = GetOper(tokens, current_token_num);

	SYN_ASSERT(oper_node != nullptr && expr_node != nullptr);

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
		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);
		return assign_node;
	}

	if (TOKEN_TYPE == OPER && TOKEN_OPER == OFBR)
	{
		(*current_token_num)++;
		TreeNode* oper_node1 = GetOper(tokens, current_token_num);
		TreeNode* oper_node2 = nullptr;
		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);
		(*current_token_num)++;

		while(oper_node2 = GetOper(tokens, current_token_num))
		{
			size_t sep_token_num = *current_token_num;
			SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);

			tokens[sep_token_num]->left  = oper_node1;
			tokens[sep_token_num]->right = oper_node2;

			oper_node1 = tokens[sep_token_num];

			(*current_token_num)++;
		}
	
		SYN_ASSERT(TOKEN_OPER == CFBR);
		(*current_token_num)++;

		return oper_node1;
	}

	return nullptr;
}

TreeNode* GetProgram(TreeNode** tokens, size_t* current_token_num)
{
	TreeNode* oper_node1 = GetOper(tokens, current_token_num);
	TreeNode* oper_node2 = nullptr;
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);
	(*current_token_num)++;

	while(oper_node2 = GetOper(tokens, current_token_num))
	{
		size_t sep_token_num = *current_token_num;
		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);

		tokens[sep_token_num]->left  = oper_node1;
		tokens[sep_token_num]->right = oper_node2;

		oper_node1 = tokens[sep_token_num];

		(*current_token_num)++;
	} 

	return oper_node1;
}

/****************************************************************************************************************
* 
* RECURSIVE DESCENT BLOCK END
* 
*****************************************************************************************************************/


Tree GetCodeTree(const char* file_name, NameTable* nametable)
{ 
	TreeNode** tokens    = nullptr;
	Tree       expr_tree = {};
	TreeCtor(&expr_tree, ASTTreeElemCtor, ASTTreeElemDtor, ASTTreeElemPrinter);

	tokens = DoLexicalAnalisys(&expr_tree, file_name, nametable);

	size_t current_token_num = 0;
	expr_tree.root = GetProgram(tokens, &current_token_num);

	return expr_tree;
}