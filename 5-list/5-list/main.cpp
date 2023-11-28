#include "list.h"

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
	//ListDump(&list, stderr);

	TestList(&list);

	ListGraphPrint(&list);

	ListDtor(&list);

	return 0;
}