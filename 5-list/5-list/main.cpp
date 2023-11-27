#include "list.h"

void TestList(List* list)
{
	ListInsertAfter(list, list->tail, 1);
	//ListDump(list, stderr);

	ListInsertAfter(list, list->tail, 2);
	//ListDump(list, stderr);

 	ListInsertAfter(list, list->tail, 4);
	//ListDump(list, stderr);
}

int main()
{
	List list = {};
	
	ListCtor(&list);
	//ListDump(&list, stderr);

	TestList(&list);

	ListGraphDump(&list);

	ListDtor(&list);

	return 0;
}