#ifndef STACK_IO_H
#define STACK_IO_H

#include "stack.h"

#define PRINT_DATA_TO_FILE(file, i, stk_data_elem)  fprintf(file, "  [%zu] = " STACK_ELEM_T_SPECIFIER, i, stk_data_elem)
#define PRINT_DATA(i, stk_data_elem)                PRINT_DATA_TO_FILE(stdout, i, stk_data_elem)

void PrintStackElems(Stack* stk);

const int LOG_FILE_ERROR = 0xBADF11E;

#ifdef _DEBUG

int LogFileCtor(FILE** log_file, const char* file_path);
int LogFileDtor(FILE* log_file);

#define STACK_DUMP_TO_FILE(stk) StackDumpToFile (stk, __FILE__, __LINE__, __func__)

void PrintStackErrorsToFile(unsigned errors);

void StackDumpToFile(Stack* stk, const char* file_name, const size_t line, const char* func_name);

#endif // _DEBUG

#endif // STACK_IO_H