#include <stdio.h>

#include "io.h"

#ifdef _DEBUG
void PrintStackErrorsToFile(unsigned errors, FILE* log_file)
{
    if (errors == 0)
        fprintf(log_file, "-----------NO_ERRORS-----------\n");
    else
    {
        fprintf(log_file, "-------------ERRORS------------\n");
        if (errors & STACK_DATA_POINTER_NULL)     fprintf(log_file, "Stack DATA NULL\n");
        if (errors & STACK_LCANARY_DMG)           fprintf(log_file, "Stack Left Canary DAMAGED\n");
        if (errors & STACK_RCANARY_DMG)           fprintf(log_file, "Stack Right Canary DAMAGED\n");
        if (errors & STACK_SIZE_GREATER_CAPACITY) fprintf(log_file, "Stack Size GREATER Capacity\n");
        if (errors & STACK_SIZE_LESS_ONE)         fprintf(log_file, "Stack Size LESS One\n");
        if (errors & STACK_DATA_LCANARY_DMG)      fprintf(log_file, "Stack Data Left Canary DAMAGED\n");
        if (errors & STACK_DATA_RCANARY_DMG)      fprintf(log_file, "Stack Data Right Canary DAMAGED\n");
        if (errors & STACK_HASH_MISMATCH)         fprintf(log_file, "Stack Hash DIFFERENCE\n");
        if (errors & STACK_REALLOC_ERROR)         fprintf(log_file, "Stack REALLOC ERROR\n");
        if (errors & STACK_BAD_SIZE)              fprintf(log_file, "Stack Size TOO BIG\n");
        if (errors & STACK_BAD_CAPACITY)          fprintf(log_file, "Stack Capacity TOO BIG\n");
        fprintf(log_file, "----------END_OF_ERRORS--------\n");
    }
}

int StackDumpToFile(Stack* stk, const char* file_name, const size_t line, const char* func_name)
{
    static size_t num_dump_call = 1;
    FILE* log_file = NULL;

    if (num_dump_call == 1)
        fopen_s(&log_file, "log.txt", "w");
    else
        fopen_s(&log_file, "log.txt", "a");

    if (!log_file)
    {
        printf("OPENING or CREATING FILE ERROR\n");
        return -1;
    }

    fprintf(log_file, "=======================================\n"
    "STACK DUMP CALL #%zu\n"
    "Called from: %s(%zu)\n", num_dump_call++, func_name, line);
    if (!stk)
    {
        fprintf(log_file, "Stack Pointer is NULL. No data in Stack\n"
        "=======================================\n\n");
        fclose(log_file);
        return 0;
    }
    PrintStackErrorsToFile(stk->errors, log_file);

    fprintf(log_file, "Stack [%p], name: \"%s\" from: %s(%zu)\n",
        stk,
        stk->stack_creation_inf.var_name,
        stk->stack_creation_inf.func,
        stk->stack_creation_inf.line);
    fprintf(log_file, "{\n");
    fprintf(log_file, "size = %zu\n", stk->size);
    fprintf(log_file, "capacity = %zu\n", stk->capacity);
    if (!(stk->errors & STACK_DATA_POINTER_NULL))
    {
        fprintf(log_file, "data[%p]\n", stk->data);
        fprintf(log_file, "  {\n");
        for (size_t i = 0; i < stk->capacity; i++)
        {
            PRINT_DATA_TO_FILE(log_file, i, stk->data[i]);
            if (stk->data[i] == POISON_ELEM)
                fprintf(log_file, " ( POISON )");
            fprintf(log_file, "\n");
        }
        fprintf(log_file, "  }\n");
    }
    fprintf(log_file, "}\n"
    "=======================================\n\n");

    fclose(log_file);
    return 0;
}

#endif
void PrintStackElems(Stack* stk)
{
	for (size_t i = 0; i < stk->size; i++)
	{
		PRINT_DATA(i, stk->data[i]);
		printf("\n");
	}
}