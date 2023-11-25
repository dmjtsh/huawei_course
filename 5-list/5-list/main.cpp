#include "list.h"

void TestList(List* list)
{
	ListInsertAfter(list, list->tail, 1);
	//ListDump(list, stderr);

	ListGraphDump(list);

	ListInsertAfter(list, list->tail, 2);
	//ListDump(list, stderr);

	ListInsertAfter(list, list->head, 4);
	//ListDump(list, stderr);

	ListRemove(list, 1);
	//ListDump(list, stderr);

	ListInsertBefore(list, list->tail, 5);
	//ListDump(list, stderr);

	ListInsertAfter(list, list->tail, 228);
}

int main()
{
	List list = {};
	
	ListCtor(&list);
	//ListDump(&list, stderr);

	TestList(&list);

	ListDtor(&list);

	return 0;
}