#include <assert.h>

#include "../../common/nametable.h"
#include "../../common/DimasLIB/DimasTree/tree.h"

#define NODE_OPER current_node->node_elem.elem.oper
#define NODE_TYPE current_node->node_elem.type 

#define RETRANSLATE_BINARY_OP(operation)								  \
	RetranslateNode(tree, current_node->left, nametable, asm_code_file);  \
	RetranslateNode(tree, current_node->right, nametable, asm_code_file); \
	fprintf(asm_code_file, #operation "\n");                              \

void RetranslateNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file);

void RetranslateWhileNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(current_node  != nullptr);
	assert(nametable     != nullptr);
	assert(asm_code_file != nullptr);

	static size_t while_count  = 1;
	size_t current_while_count = while_count;

	fprintf(asm_code_file, "\nstartwhile%zu:\n", current_while_count);

	RetranslateNode(tree, current_node->left, nametable, asm_code_file);

	fprintf(asm_code_file, "PUSH 0\n");
	fprintf(asm_code_file, "JE endwhile%zu\n", current_while_count); // TODO: static var
	
	RetranslateNode(tree, current_node->right, nametable, asm_code_file);

	fprintf(asm_code_file, "JMP startwhile%zu\n", current_while_count);
	fprintf(asm_code_file, "endwhile%zu:\n", current_while_count);

	while_count++;
}

void RetranslateIfNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(current_node  != nullptr);
	assert(nametable     != nullptr);
	assert(asm_code_file != nullptr);

	static size_t if_count  = 1;
	size_t current_if_count = if_count;

	fprintf(asm_code_file, "\nstartif%zu:\n", current_if_count);

	RetranslateNode(tree, current_node->left, nametable, asm_code_file);

	fprintf(asm_code_file, "PUSH 0\n");
	fprintf(asm_code_file, "JE endif%zu\n", current_if_count); // TODO: static var
	
	RetranslateNode(tree, current_node->right, nametable, asm_code_file);

	fprintf(asm_code_file, "endif%zu:\n", current_if_count);

	if_count++;
}

void RetranslateAssignNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(current_node  != nullptr);
	assert(nametable     != nullptr);
	assert(asm_code_file != nullptr);

	RetranslateNode(tree, current_node->right, nametable, asm_code_file);

	fprintf(asm_code_file, "POP [%d] \t; %s\n", current_node->left->node_elem.elem.var->code, current_node->left->node_elem.elem.var->str);
}

void RetranslateNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(nametable     != nullptr);
	assert(asm_code_file != nullptr);

	if(!current_node)
		return;

	if (NODE_TYPE == OPER)
	{
		switch(NODE_OPER)
		{
			case WHILE:
				RetranslateWhileNode(tree, current_node, nametable, asm_code_file);
				return;
			case ASSIGN:
				RetranslateAssignNode(tree, current_node, nametable, asm_code_file);
				return;
			case IF:
				RetranslateIfNode(tree, current_node, nametable, asm_code_file);
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

	RetranslateNode(tree, current_node->left,  nametable, asm_code_file);
	RetranslateNode(tree, current_node->right, nametable, asm_code_file);

	switch(current_node->node_elem.type)
	{
		case NUM:
			fprintf(asm_code_file, "PUSH %lg\n", current_node->node_elem.elem.num);
			break;
		case VAR:

			fprintf(asm_code_file, "PUSH [%d] \t; %s\n", current_node->node_elem.elem.var->code, current_node->node_elem.elem.var->str);
			break;
		case OPER:
			break;
		default:
			assert(0);
	}
}

void RetranslateTree(Tree* tree, NameTable* nametable, FILE* asm_code_file)
{
	assert(tree          != nullptr);
	assert(nametable     != nullptr);
	assert(asm_code_file != nullptr);

	RetranslateNode(tree, tree->root, nametable, asm_code_file);
}

#undef RETRANSLATE_BINARY_OP