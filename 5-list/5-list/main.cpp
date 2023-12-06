#define _CRT_SECURE_NO_WARNINGS
#include "list.h"
#include "list_io.h"

void TestList(List* list)
{
	ListInsertBack(list, 1);
	//ListDump(list, stderr);

	ListInsertBack(list, 2);
	//ListDump(list, stderr);

 	ListInsertBack(list, 4);
	//ListDump(list, stderr);
}

int main()
{
	List list = {};
	
	ListCtor(&list);
	list.logger = fopen("logger.html", "w");
	list.graph  = fopen("list_graph.gv", "w");
	fprintf(list.logger, "<pre>\n");
	//ListDump(&list, stderr);

	TestList(&list);

	ListGraphPrint(&list, "ded loh");

	ListDtor(&list);

	return 0;
}