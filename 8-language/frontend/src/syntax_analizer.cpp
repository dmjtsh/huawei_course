#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../../common/DimasLIB/DimasUtilities/utilities.h"
#include "../../common/DimasLIB/DimasTree/tree.h"
#include "../../common/ast_tree.h"
#include "../../common/program_nametables.h"
#include "lexical_analizer.h"

#define TOKEN_TYPE				 tokens[*current_token_num]->node_elem.type
#define TOKEN_OPER				 tokens[*current_token_num]->node_elem.elem.oper
#define TOKEN_NUM				 tokens[*current_token_num]->node_elem.elem.num
#define TOKEN_ID_STR			 tokens[*current_token_num]->node_elem.elem.id->str
#define INC_TOKEN_NUM            (*current_token_num)++
#define DEC_TOKEN_NUM			 (*current_token_num)--

#define GET_NUM(...)         GetN                 (tokens, current_token_num, __VA_ARGS__)
#define GET_ID(...)          GetId                (tokens, current_token_num, __VA_ARGS__)
#define GET_EXPR(...)        GetExpr              (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_OPER(...)        GetOper              (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_PRIMARY(...)     GetPrimary           (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_TERM(...)        GetTerm              (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_ASSIGN(...)      GetAssign            (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_FUNC_ARGS(...)   GetFunctionArgs      (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_FUNC_CALL(...)   GetFunctionCall      (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_IF(...)          GetIf                (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_WHILE(...)       GetWhile             (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_OPER(...)        GetOper              (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_FUNC_PARAMS(...) GetFunctionParams    (tokens, current_token_num, nametables, __VA_ARGS__)
#define GET_FUNC_DEF(...)    GetFunctionDefinition(tokens, current_token_num, nametables, __VA_ARGS__)

 //---/\---/\-------Это ASCII KOT!--//
 //  {  '-'  }                      //
 //  {  0 0  }     Добавь его себе  //
 //  --> V <--  в исходник, и тебе  //
 //   \ \|/ /      будет, наверно,  //
 //    \___/  приятно отлаживаться  //
 //---------------долгими ночами:)--//

void PrintCurrentToken(TreeNode** tokens, size_t* current_token_num)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);

	switch(TOKEN_TYPE)
	{
		case ID:
			printf("%s\n", TOKEN_ID_STR);
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

TreeNode* GetExpr(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables);
TreeNode* GetOper(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables);

TreeNode* GetN(TreeNode** tokens, size_t* current_token_num)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);

	if(TOKEN_TYPE == NUM)
	{
		INC_TOKEN_NUM;
		return tokens[*current_token_num-1];
	}

	return nullptr;
}

TreeNode* GetId(TreeNode** tokens,  size_t* current_token_num)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);

	if(tokens[*current_token_num]->node_elem.type == ID)
	{
		INC_TOKEN_NUM;
		return tokens[*current_token_num-1];
	}

	return nullptr;
}

TreeNode* GetPrimary(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);	
	
	if (TOKEN_TYPE == OPER && TOKEN_OPER == OBR)
	{
		INC_TOKEN_NUM;
		TreeNode* expr_token = GET_EXPR();
		
		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
		INC_TOKEN_NUM;
		
		SYN_ASSERT(expr_token != nullptr);
		return expr_token;
	}
	else if (TOKEN_TYPE == NUM)
	{
		TreeNode* num_token = GET_NUM();
	
		SYN_ASSERT(num_token != nullptr);
		return num_token;
	}
	else if (TOKEN_TYPE == ID)
	{
		TreeNode* id_token = GET_ID();

		// CHECKING IF THIS VAR IS NOT DEFINED
		SYN_ASSERT(NameTableFind(&CURRENT_SCOPES_NAMETABLE, id_token->node_elem.elem.id->str));

		SYN_ASSERT(id_token != nullptr);
		return id_token;
	}
	else
		SYN_ASSERT(0);
}

TreeNode* GetTerm(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);	
	
	TreeNode* primary_node1 = GET_PRIMARY();
	
	while(TOKEN_OPER == MUL || TOKEN_OPER == DIV)
	{
		size_t oper_token_num = *current_token_num;

		INC_TOKEN_NUM;

		TreeNode* primary_node2 = GET_PRIMARY();
		
		tokens[oper_token_num]->left  = primary_node1;
		tokens[oper_token_num]->right = primary_node2;	

		primary_node1 = tokens[oper_token_num];
	}

	return primary_node1;
}

TreeNode* GetExpr(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);	
	
	TreeNode* term_node1 = GET_TERM();
	
	while(TOKEN_OPER == ADD || TOKEN_OPER == SUB)
	{
		size_t oper_token_num = *current_token_num;

		INC_TOKEN_NUM;

		TreeNode* term_node2 = GET_TERM();
		
		tokens[oper_token_num]->left  = term_node1;
		tokens[oper_token_num]->right = term_node2;	

		term_node1 = tokens[oper_token_num];
	}

	return term_node1;
}

// TODO: АССЕРТЫ И ДЕФАЙНЫ!!!!!!!!!!!!!!!!!!!!!!!!!!
TreeNode* GetAssign(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);

	TreeNode* var_name_node = GET_ID();
	if(!var_name_node)
		return nullptr;

	if(!(TOKEN_TYPE == OPER && TOKEN_OPER == ASSIGN))
		return nullptr;

	size_t assign_token_num = *current_token_num;
	INC_TOKEN_NUM;

	// ADDING VAR TO SCOPE NAMETABLE
	char* var_name = var_name_node->node_elem.elem.id->str;
	var_name_node->node_elem.elem.id = ProgramNameTablesAddVar(nametables, var_name);
	
	tokens[assign_token_num]->left  = var_name_node;
	tokens[assign_token_num]->right = GET_EXPR();

	return tokens[assign_token_num];
}

TreeNode* GetFunctionArgs(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables, size_t* args_count)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);

	TreeNode* func_args_node1 = GET_ID();

	if(!func_args_node1)
		return nullptr;

	if(func_args_node1 && (TOKEN_TYPE == OPER && TOKEN_OPER == FUNC_SEP))
	{
		(*args_count)++;

		size_t sep_token_num = *current_token_num;
		INC_TOKEN_NUM;

		TreeNode* func_args_node2 = nullptr;

		while(func_args_node2 = GET_ID())
		{
			(*args_count)++;

			SYN_ASSERT(func_args_node2);

			tokens[sep_token_num]->left  = func_args_node1;
			tokens[sep_token_num]->right = func_args_node2;

			func_args_node1 = tokens[sep_token_num];

			if(TOKEN_TYPE == OPER && TOKEN_OPER == FUNC_SEP)
			{
				sep_token_num = *current_token_num;
				INC_TOKEN_NUM;
				continue;
			}
			else
				break;
		}
	}
	
	return func_args_node1;
}

TreeNode* GetFunctionCall(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);	
	
	TreeNode* func_name_node = GET_ID();
	if(!func_name_node)
		return nullptr;

	if(!(TOKEN_TYPE == OPER && TOKEN_OPER == OBR))
	{
		DEC_TOKEN_NUM;
		return nullptr;
	}
	else
		INC_TOKEN_NUM;

	// CHECKING IF THIS FUNC IS NOT DEFINED
	char* func_name = func_name_node->node_elem.elem.id->str;
	SYN_ASSERT(func_name_node->node_elem.elem.id = NameTableFind(&nametables->functions_nametable, func_name));

	size_t args_count = 0;
	func_name_node->left = GET_FUNC_ARGS(&args_count);
	// CHEKING THAT ARGS COUNT == PARAMS COUNT
	SYN_ASSERT(args_count == func_name_node->node_elem.elem.id->code);

	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == CBR);
	INC_TOKEN_NUM;

	return func_name_node;
}

// TODO: ВОТ ТУТА ДЕФАЙНЫ
TreeNode* GetIf(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);

	if(!(TOKEN_TYPE == OPER && TOKEN_OPER == IF))
		return nullptr;

	size_t if_token_num = *current_token_num;

	INC_TOKEN_NUM;
	
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
	INC_TOKEN_NUM;
	
	TreeNode* expr_node = GET_EXPR();
	
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == CBR);
	INC_TOKEN_NUM;

	TreeNode* oper_node = GET_OPER();

	SYN_ASSERT(oper_node != nullptr && expr_node != nullptr);

	tokens[if_token_num]->left  = expr_node;
	tokens[if_token_num]->right = oper_node;

	return tokens[if_token_num];
}

TreeNode* GetWhile(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);	
	
	if(!(TOKEN_TYPE == OPER && TOKEN_OPER == WHILE))
		return nullptr;

	size_t while_token_num = *current_token_num;

	INC_TOKEN_NUM;
	tokens[*current_token_num-1];
	tokens[*current_token_num];

	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
	INC_TOKEN_NUM;
	
	TreeNode* expr_node = GET_EXPR();
	
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == CBR);
	INC_TOKEN_NUM;

	TreeNode* oper_node = GET_OPER();

	SYN_ASSERT(oper_node != nullptr && expr_node != nullptr);

	tokens[while_token_num]->left  = expr_node;
	tokens[while_token_num]->right = oper_node;

	return tokens[while_token_num];
}

TreeNode* GetOper(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);	
	
	TreeNode* if_node     = GET_IF();
	if(if_node)
		return if_node;

	TreeNode* while_node  = GET_WHILE();
	if(while_node)
		return while_node;

	TreeNode* func_call_node = GET_FUNC_CALL();
	if(func_call_node)
	{
		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);
		return func_call_node;
	}

	TreeNode* assign_node = GET_ASSIGN();
	if(assign_node)
	{
		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);
		return assign_node;
	}

	if (TOKEN_TYPE == OPER && TOKEN_OPER == OFBR)
	{
		INC_TOKEN_NUM;
		TreeNode* oper_node1 = GET_OPER();
		TreeNode* oper_node2 = nullptr;

		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);
		INC_TOKEN_NUM;

		while(oper_node2 = GET_OPER())
		{
			size_t sep_token_num = *current_token_num;
			SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);

			tokens[sep_token_num]->left  = oper_node1;
			tokens[sep_token_num]->right = oper_node2;

			oper_node1 = tokens[sep_token_num];

			INC_TOKEN_NUM;
		}
	
		SYN_ASSERT(TOKEN_OPER == CFBR);
		INC_TOKEN_NUM;

		return oper_node1;
	}

	return nullptr;
}

TreeNode* GetFunctionParams(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables, size_t* params_count)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);

	TreeNode* func_param_node1 = GET_ID();

	if(!func_param_node1)
		return nullptr;

	// ADDING FUNC PARAMS TO SCOPE NAMETABLE
	char* func_param_name1 = func_param_node1->node_elem.elem.id->str;
	func_param_node1->node_elem.elem.id = ProgramNameTablesAddVar(nametables, func_param_name1);
	
	if(func_param_node1 && (TOKEN_TYPE == OPER && TOKEN_OPER == FUNC_SEP))
	{
		(*params_count)++;

		size_t sep_token_num = *current_token_num;
		INC_TOKEN_NUM;

		TreeNode* func_param_node2 = nullptr;

		while(func_param_node2 = GET_ID())
		{
			SYN_ASSERT(func_param_node2);
			(*params_count)++;

			// ADDING FUNC PARAMS TO SCOPE NAMETABLE
			char* func_param_name2 = func_param_node2->node_elem.elem.id->str;
			func_param_node2->node_elem.elem.id = ProgramNameTablesAddVar(nametables, func_param_name2);

			tokens[sep_token_num]->left  = func_param_node1;
			tokens[sep_token_num]->right = func_param_node2;

			func_param_node1 = tokens[sep_token_num];

			if(TOKEN_TYPE == OPER && TOKEN_OPER == FUNC_SEP)
			{
				sep_token_num = *current_token_num;
				INC_TOKEN_NUM;
				continue;
			}
			else
				break;
		}
	}

	return func_param_node1;
}

TreeNode* GetFunctionDefinition(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);

	if(!tokens[*current_token_num])
		return nullptr;

	if(!(TOKEN_TYPE == OPER && TOKEN_OPER == FUNC))
		return nullptr;

	size_t func_token_num = *current_token_num;
	INC_TOKEN_NUM;

	TreeNode* func_name_node = GET_ID();

	// ADDING FUNC TO NAMETABLE
	char* func_name = func_name_node->node_elem.elem.id->str;
	SYN_ASSERT(func_name_node->node_elem.elem.id = ProgramNameTablesAddFunc(nametables, func_name));

	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == OBR);
	INC_TOKEN_NUM;

	size_t params_count = 0;
	TreeNode* func_param_node = GetFunctionParams(tokens, current_token_num, nametables, &params_count);
	// SETTING PARAMS COUNT
	func_name_node->node_elem.elem.id->code = params_count;

	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == CBR);
	INC_TOKEN_NUM;

	TreeNode* oper_node = GET_OPER();

	tokens[func_token_num]->left       = func_name_node;
	tokens[func_token_num]->left->left = func_param_node; 
	tokens[func_token_num]->right      = oper_node;

	return tokens[func_token_num];
}


// TODO: Везде Ассерты расставить
TreeNode* GetProgram(TreeNode** tokens, size_t* current_token_num, ProgramNameTables* nametables)
{
	assert(tokens            != nullptr);
	assert(current_token_num != nullptr);
	assert(nametables        != nullptr);

	TreeNode* func_node1 = GetFunctionDefinition(tokens, current_token_num, nametables);
	TreeNode* func_node2 = nullptr;
	
	SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);
	INC_TOKEN_NUM;

	while(func_node2 = GetFunctionDefinition(tokens, current_token_num, nametables))
	{
		size_t sep_token_num = *current_token_num;
		SYN_ASSERT(TOKEN_TYPE == OPER && TOKEN_OPER == SEPARATOR);

		tokens[sep_token_num]->left  = func_node1;
		tokens[sep_token_num]->right = func_node2;

		func_node1 = tokens[sep_token_num];

		INC_TOKEN_NUM;
	} 

	return func_node1;
}

/****************************************************************************************************************
* 
* RECURSIVE DESCENT BLOCK END
* 
*****************************************************************************************************************/

Tree GetCodeTree(const char* file_name, ProgramNameTables* nametables)
{ 
	assert(file_name  != nullptr);
	assert(nametables != nullptr);

	TreeNode** tokens    = nullptr;
	Tree       expr_tree = {};
	TreeCtor(&expr_tree, ASTTreeElemCtor, ASTTreeElemDtor, ASTTreeElemPrinter);

	size_t scopes_counter = 0;
	tokens = DoLexicalAnalisys(&expr_tree, file_name, &nametables->common_nametable, &scopes_counter);

	ProgramNameTablesCtor(nametables, scopes_counter);

	size_t current_token_num = 0;
	expr_tree.root = GetProgram(tokens, &current_token_num, nametables);

	return expr_tree;
}