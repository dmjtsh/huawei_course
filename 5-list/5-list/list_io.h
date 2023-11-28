#ifndef LIST_IO_H
#define LIST_IO_H

#include "list.h"

void ListPrint(List* list, FILE* logger);

void ListGraphPrint(List* list);

void ListDump(List* list, FILE* logger);

#endif