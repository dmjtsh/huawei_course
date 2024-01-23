#define _CRT_SECURE_NO_WARNINGS

#include "../../common/TX/TXLib.h"
#include "../../common/nametable.h"
#include "../../common/ast_tree.h"
#include "syntax_analizer.h"

#include <locale.h>

int main(int argc, char* argv[])
{	
	NameTable nametable = {};
	NameTableCtor(&nametable);
	Tree tree = GetCodeTree((argc > 1)? argv[1] : "source.txt", &nametable);

	TreeGraphPrint(&tree, "expr_tree");

	FILE* ast_file = fopen((argc > 2)? argv[2] : "database.txt", "w");

	WriteNameTable(&nametable, ast_file);
	WriteASTTree(&tree, ast_file);

	NameTableDtor(&nametable);
	TreeDtor(&tree);

	return 0;
}	