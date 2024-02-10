#define _CRT_SECURE_NO_WARNINGS

#include "../../common/DimasLIB/DimasUtilities/utilities.h"
#include "../../common/TX/TXLib.h"
#include "../../common/program_nametables.h"
#include "../../common/ast_tree.h"
#include "retranslator.h"

#include <locale.h>

int main(int argc, char* argv[])
{	
	ProgramNameTables nametables = {};
	
	Tree tree = {};
	TreeCtor(&tree, ASTTreeElemCtor, ASTTreeElemDtor, ASTTreeElemPrinter);

	const char* ast_file_name = (argc > 2)? argv[1] : "../../frontend/src/database.txt"; 

	ReadTreeAndNameTables(&nametables, &tree, ast_file_name);

	TreeGraphPrint(&tree, "expr_tree");

	RetranslateTree(&tree, &nametables, stdout);
	
	ProgramNameTablesDtor(&nametables);
	TreeDtor(&tree);

	return 0;
}	