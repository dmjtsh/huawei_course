#include <assert.h>

#include "../../common/nametable.h"
#include "../../common/program_nametables.h"
#include "../../common/DimasLIB/DimasTree/tree.h"

#define NODE_OPER current_node->node_elem.elem.oper
#define NODE_TYPE current_node->node_elem.type 

#define RETRANSLATE_BINARY_OP(operation)								   \
	RetranslateNode(tree, current_node->left, nametables, asm_code_file);  \
	RetranslateNode(tree, current_node->right, nametables, asm_code_file); \
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

	RetranslateNode(tree, current_node->left, nametables, asm_code_file);

	fprintf(asm_code_file, "\tPUSH 0\n");
	fprintf(asm_code_file, "\tJE endwhile%zu\n", current_while_count); // TODO: static var
	
	RetranslateNode(tree, current_node->right, nametables, asm_code_file);

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

	RetranslateNode(tree, current_node->left, nametables, asm_code_file);

	fprintf(asm_code_file, "\tPUSH 0\n");
	fprintf(asm_code_file, "\tJE endif%zu\n", current_if_count); // TODO: static var
	
	RetranslateNode(tree, current_node->right, nametables, asm_code_file);

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

	RetranslateNode(tree, current_node->right, nametables, asm_code_file);

	fprintf(asm_code_file, "\n\tRET\n");
}

void RetranslateAssignNode(Tree* tree, TreeNode* current_node, ProgramNameTables* nametables, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(current_node  != nullptr);
	assert(nametables     != nullptr);
	assert(asm_code_file != nullptr);

	RetranslateNode(tree, current_node->right, nametables, asm_code_file);

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
			default:
				break;
		}
	}

	RetranslateNode(tree, current_node->left,  nametables, asm_code_file);
	RetranslateNode(tree, current_node->right, nametables, asm_code_file);

	switch(current_node->node_elem.type)
	{
		case NUM:
			fprintf(asm_code_file, "\tPUSH %lg\n", current_node->node_elem.elem.num);
			break;
		case ID:
			if(current_node->node_elem.elem.id->type == FUNCTION)
			{
				size_t     func_scope_number = NameTableFind(&nametables->functions_nametable, current_node->node_elem.elem.id->str)->code;
				NameTable* curr_scope_nametable = &nametables->scopes_nametables[func_scope_number];

				fprintf(asm_code_file, "\tPOP [%d] \t; %s\n", curr_scope_nametable->elems[0].code, curr_scope_nametable->elems[0].str); // TODO: ADD >1 VARS
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

#undef RETRANSLATE_BINARY_OP