#define _CRT_SECURE_NO_WARNINGS

#include "../../common/DimasLIB/DimasUtilities/utilities.h"
#include "../../common/TX/TXLib.h"
#include "../../common/nametable.h"
#include "../../common/ast_tree.h"
#include "retranslator.h"

#include <locale.h>

FILE* ded32hui = NULL; 

int main(int argc, char* argv[])
{	
	NameTable nametable = {};
	NameTableCtor(&nametable);
	
	Tree tree = {};
	TreeCtor(&tree, ASTTreeElemCtor, ASTTreeElemDtor, ASTTreeElemPrinter);

	const char* ast_file_name = (argc > 2)? argv[1] : "../../frontend/src/database.txt"; 

	FILE* ast_file = fopen(ast_file_name, "r");
	      ded32hui = fopen(ast_file_name, "r");
	assert(ast_file);

	unsigned errors = 0;

	setvbuf(ast_file, nullptr, _IOFBF, GetFileSize(ast_file_name));
	ReadNameTable(&nametable, ast_file, &errors);
	ReadASTTree(&tree, &nametable, ast_file, &errors);

	TreeGraphPrint(&tree, "expr_tree");

	RetranslateTree(&tree, &nametable, stdout);
	
	NameTableDtor(&nametable);
	TreeDtor(&tree);

	return 0;
}	