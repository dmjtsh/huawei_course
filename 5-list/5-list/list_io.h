#ifndef LIST_IO_H
#define LIST_IO_H

#include "list.h"

const char* ListGraphPrint(List* list, const char* reason_blya);

void ListDump(List* list, FILE* logger);

#endif