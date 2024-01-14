#include <assert.h>

#include "nametable.h"
#include "../frontend/src/syntax_analizer.h"
#include "DimasLIB/DimasTree/tree.h"

void RetranslateNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file);

void RetranslateWhileNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file)
{
	fprintf(asm_code_file, "\nstartwhile:\n");

	RetranslateNode(tree, current_node->left, nametable, asm_code_file);

	fprintf(asm_code_file, "PUSH 0\n");
	fprintf(asm_code_file, "JE endwhile\n"); // TODO: static var
	
	RetranslateNode(tree, current_node->right, nametable, asm_code_file);

	fprintf(asm_code_file, "JMP startwhile\n");
	fprintf(asm_code_file, "endwhile:\n");
}

void RetranslateAssignNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file)
{
	RetranslateNode(tree, current_node->right, nametable, asm_code_file);

	fprintf(asm_code_file, "POP [%s]\n", current_node->left->node_elem.elem.var.name);
}

void RetranslateNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file)
{
	if(!current_node)
		return;

	if(current_node->node_elem.elem.oper == WHILE)
	{
		RetranslateWhileNode(tree, current_node, nametable, asm_code_file);
		return;
	}
	if(current_node->node_elem.elem.oper == ASSIGN)
	{
		RetranslateAssignNode(tree, current_node, nametable, asm_code_file);
		return;
	}

	RetranslateNode(tree, current_node->left,  nametable, asm_code_file);
	if (current_node->node_elem.elem.oper == IF)
	{
		fprintf(asm_code_file, "PUSH 0\n");
		fprintf(asm_code_file, "JE endif\n"); // TODO: static var
	}
	RetranslateNode(tree, current_node->right, nametable, asm_code_file);

	switch(current_node->node_elem.type)
	{
	case NUM:
		fprintf(asm_code_file, "PUSH %lg\n", current_node->node_elem.elem.num);
		break;
	case VAR:
		fprintf(asm_code_file, "PUSH [%s]\n", current_node->node_elem.elem.var.name);
		break;
	case OPER:
		switch(current_node->node_elem.elem.oper)
		{
			case IF:
				fprintf(asm_code_file, "endif:\n");
		}
		break;
	}
}