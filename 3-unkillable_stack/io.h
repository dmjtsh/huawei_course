#ifndef IO_H
#define IO_H

#include "stack.h"

#define PRINT_DATA(i, stk_data_elem)                printf("  [%zu] = " ELEM_T_SPECIFIER, i, stk_data_elem)


#ifdef _DEBUG
#define PRINT_DATA_TO_FILE(file, i, stk_data_elem)  fprintf(file, "  [%zu] = " ELEM_T_SPECIFIER, i, stk_data_elem)
#define STACK_DUMP_TO_FILE(stk)                     StackDumpToFile (stk, __FILE__, __LINE__, __func__)

void PrintStackErrorsToFile(unsigned errors, FILE* log_file);

int StackDumpToFile(Stack* stk, const char* file_name, const size_t line, const char* func_name);
#endif

void PrintStackElems(Stack* stk);

#endif