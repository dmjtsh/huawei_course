#define _CRT_SECURE_NO_WARNINGS
#include "frontend.h"

#include <locale.h>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");

	Tree tree = {};

	if (argc > 1)
		tree = GetCodeTree(argv[1]);
	else
		tree = GetCodeTree("source.txt");

	TreeGraphPrint(&tree, "lol");

	TreeDtor(&tree);
	return 0;
}	