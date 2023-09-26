#ifndef IO_H
#define IO_H

#include "stack.h"

#ifdef _DEBUG
#define STACK_DUMP(stk)               StackDump (stk, __FILE__, __LINE__, __func__)
#define PRINT_DATA(i, stk_data_elem)  printf("  [%zu] = %d\n", i, stk_data_elem)

void PrintStackErrors(unsigned errors);

int StackDump(Stack* stk, const char* file_name, const size_t line, const char* func_name);
#endif

void PrintStackElems(Stack* stk);

#endif