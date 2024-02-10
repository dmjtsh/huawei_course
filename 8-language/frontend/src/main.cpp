#define _CRT_SECURE_NO_WARNINGS

#include "syntax_analizer.h"
#include "../../common/TX/TXLib.h"
#include "../../common/nametable.h"
#include "../../common/ast_tree.h"
#include "../../common/program_nametables.h"

#include <locale.h>

int main(int argc, char* argv[])
{	
	ProgramNameTables nametables = {};

	// TODO: сделать либо контекст либо SetCodeTree 
	Tree tree = GetCodeTree((argc > 1)? argv[1] : "source.txt", &nametables);

	TreeGraphPrint(&tree, "expr_tree");

	FILE* ast_file = fopen((argc > 2)? argv[2] : "database.txt", "w");

	WriteProgramNameTables(&nametables, ast_file);
	WriteASTTree(&tree, ast_file);

	ProgramNameTablesDtor(&nametables);
	TreeDtor(&tree);

	return 0;
}	