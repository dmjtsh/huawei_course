#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>

#include "../../common/nametable.h"
#include "../../common/program_nametables.h"
#include "../../common/ast_tree.h"
#include "../../common/DimasLIB/DimasTree/tree.h"
#include "../../common/DimasLIB/DimasUtilities/utilities.h"

#define NODE_OPER current_node->node_elem.elem.oper
#define NODE_TYPE current_node->node_elem.type 

#define RETRANSLATE_LEFT_NODE()  RetranslateNode(tree, current_node->left,  nametables, asm_code_file)
#define RETRANSLATE_RIGHT_NODE() RetranslateNode(tree, current_node->right, nametables, asm_code_file)

#define RETRANSLATE_BINARY_OP(operation)								   \
	RETRANSLATE_LEFT_NODE();											   \
	RETRANSLATE_RIGHT_NODE();											   \
	fprintf(asm_code_file, "\t" #operation "\n");                          \

void RetranslateNode(Tree* tree, TreeNode* current_node, ProgramNameTables* nametables, FILE* asm_code_file);

void RetranslateWhileNode(Tree* tree, TreeNode* current_node, ProgramNameTables* nametables, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(current_node  != nullptr);
	assert(nametables    != nullptr);
	assert(asm_code_file != nullptr);

	static size_t while_count  = 1;
	size_t current_while_count = while_count;

	fprintf(asm_code_file, "\n\tstartwhile%zu:\n", current_while_count);

	RETRANSLATE_LEFT_NODE();

	fprintf(asm_code_file, "\tPUSH 0\n");
	fprintf(asm_code_file, "\tJE endwhile%zu\n", current_while_count); // TODO: static var
	
	RETRANSLATE_RIGHT_NODE();

	fprintf(asm_code_file, "\tJMP startwhile%zu\n", current_while_count);
	fprintf(asm_code_file, "\tendwhile%zu:\n", current_while_count);

	while_count++;
}

void RetranslateIfNode(Tree* tree, TreeNode* current_node, ProgramNameTables* nametables, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(current_node  != nullptr);
	assert(nametables    != nullptr);
	assert(asm_code_file != nullptr);

	static size_t if_count  = 1;
	size_t current_if_count = if_count;

	fprintf(asm_code_file, "\n\tstartif%zu:\n", current_if_count);

	RETRANSLATE_LEFT_NODE();

	fprintf(asm_code_file, "\tPUSH 0\n");
	fprintf(asm_code_file, "\tJE endif%zu\n", current_if_count); // TODO: static var
	
	RETRANSLATE_RIGHT_NODE();

	fprintf(asm_code_file, "\tendif%zu:\n", current_if_count);

	if_count++;
}

void RetranslateFuncDefNode(Tree* tree, TreeNode* current_node, ProgramNameTables* nametables, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(current_node  != nullptr);
	assert(nametables    != nullptr);
	assert(asm_code_file != nullptr);

	fprintf(asm_code_file, "\n%s:\n", current_node->left->node_elem.elem.id->str);

	RETRANSLATE_RIGHT_NODE();

	fprintf(asm_code_file, "\n\tRET\n");
}

void RetranslateAssignNode(Tree* tree, TreeNode* current_node, ProgramNameTables* nametables, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(current_node  != nullptr);
	assert(nametables     != nullptr);
	assert(asm_code_file != nullptr);

	RETRANSLATE_RIGHT_NODE();

	fprintf(asm_code_file, "\tPOP [%d] \t; %s\n", current_node->left->node_elem.elem.id->code, current_node->left->node_elem.elem.id->str);
}

void RetranslateNode(Tree* tree, TreeNode* current_node, ProgramNameTables* nametables, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(nametables    != nullptr);
	assert(asm_code_file != nullptr);

	if(!current_node)
		return;

	if (NODE_TYPE == OPER)
	{
		switch(NODE_OPER)
		{
			case WHILE:
				RetranslateWhileNode(tree, current_node, nametables, asm_code_file);
				return;
			case ASSIGN:
				RetranslateAssignNode(tree, current_node, nametables, asm_code_file);
				return;
			case IF:
				RetranslateIfNode(tree, current_node, nametables, asm_code_file);
				return;
			case FUNC:
				RetranslateFuncDefNode(tree, current_node, nametables, asm_code_file);
				return;
			case ADD:
				RETRANSLATE_BINARY_OP(ADD);
				return;
			case MUL:
				RETRANSLATE_BINARY_OP(MUL);
				return;
			case SUB:
				RETRANSLATE_BINARY_OP(SUB);
				return;
			case DIV:
				RETRANSLATE_BINARY_OP(DIV);
				return;
			case SEPARATOR:
				break;
			case FUNC_SEP:
				break;
			default:
				break;
		}
	}

	RETRANSLATE_LEFT_NODE();
	RETRANSLATE_RIGHT_NODE();

	switch(current_node->node_elem.type)
	{
		case NUM:
			fprintf(asm_code_file, "\tPUSH %lg\n", current_node->node_elem.elem.num);
			break;
		case ID:
			if(current_node->node_elem.elem.id->type == FUNCTION)
			{
				size_t         func_scope_number = 0;
				char*          func_str          = current_node->node_elem.elem.id->str;
				NameTableElem* func              = NameTableFind(&nametables->functions_nametable, func_str, &func_scope_number);
				size_t         func_params_count = func->code;
				
				NameTable* curr_scope_nametable  = &nametables->scopes_nametables[func_scope_number];

				for(size_t i = 0; i < func_params_count; i++)
				{
					fprintf(asm_code_file, "\tPOP [%d] \t; %s\n", curr_scope_nametable->elems[i].code, curr_scope_nametable->elems[i].str);
				}
				fprintf(asm_code_file, "\tCALL %s\n", current_node->node_elem.elem.id->str);
			}
			else if (current_node->node_elem.elem.id->type == VARIABLE)
				fprintf(asm_code_file, "\tPUSH [%d] \t; %s\n", current_node->node_elem.elem.id->code, current_node->node_elem.elem.id->str);
			else
				assert(0);
			break;
		case OPER:
			break;
		default:
			assert(0);
	}
}

void RetranslateTree(Tree* tree, ProgramNameTables* nametables, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(nametables    != nullptr);
	assert(asm_code_file != nullptr);

	RetranslateNode(tree, tree->root, nametables, asm_code_file);
}

void ReadTreeAndNameTables(ProgramNameTables* nametables, Tree* tree, const char* ast_file_name)
{
	assert(nametables    != nullptr);
	assert(tree          != nullptr);
	assert(ast_file_name != nullptr);

	FILE* ast_file = fopen(ast_file_name, "r");
	assert(ast_file);

	unsigned errors = 0;

	setvbuf(ast_file, nullptr, _IOFBF, GetFileSize(ast_file_name));

	ReadProgramNameTables(nametables, ast_file, &errors);
	ReadASTTree(tree, nametables, ast_file, &errors);
}

#undef RETRANSLATE_BINARY_OP