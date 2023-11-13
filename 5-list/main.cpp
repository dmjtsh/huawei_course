#include "list.h"

void TestList(List* list)
{
	ListInsertAfter(list, list->tail, 1);
	ListInsertAfter(list, list->tail, 2);
	ListInsertAfter(list, list->head, 4);
	ListRemove(list, 1);
	ListInsertBefore(list, list->tail, 5);
	ListInsertAfter(list, list->tail, 228);

	ListDump(list, list->logger);
}

int main()
{
	List list = {};
	
	ListCtor(&list);

	TestList(&list);

	ListDtor(&list);

	return 0;
}