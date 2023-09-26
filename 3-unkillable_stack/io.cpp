#include <stdio.h>
#include "stack.h"
#include "io.h"

#ifdef _DEBUG
void PrintStackErrors(unsigned errors)
{
	if (errors & STACK_LCANARY_DMG)           printf("\033[31mStack Left Canary DAMAGED\033[0m\n");
	if (errors & STACK_RCANARY_DMG)           printf("\033[31mStack Right Canary DAMAGED\033[0m\n");
	if (errors & STACK_SIZE_GREATER_CAPACITY) printf("\033[31mStack Size GREATER Capacity\033[0m\n");
	if (errors & STACK_SIZE_LESS_ONE)         printf("\033[31mStack Size LESS One\033[0m\n");
	if (errors & STACK_DATA_LCANARY_DMG)      printf("\033[31mStack Data Left Canary DAMAGED\033[0m\n");
	if (errors & STACK_DATA_RCANARY_DMG)      printf("\033[31mStack Data Right Canary DAMAGED\033[0m\n");
	if (errors & STACK_HASH_MISMATCH)         printf("\033[31mStack Hash DIFFERENCE\033[0m\n");
	if (errors & STACK_REALLOC_ERROR)		  printf("\033[31mStack REALLOC ERROR\033[0m\n");
	if (errors & STACK_BAD_SIZE)		      printf("\033[31mStack Size TOO BIG\033[0m\n");
	if (errors & STACK_BAD_CAPACITY)		  printf("\033[31mStack Capacity TOO BIG\033[0m\n");
}

int StackDump(Stack* stk, const char* file_name, const size_t line, const char* func_name)
{
	printf("\n\033[32mOoops, something wrong happend, that called StackDump\033[0m\n");
	printf("called from func: %s(%zu) file: %s\n", func_name, line, file_name);
	if (!stk)
	{
		printf("\033[31mStack Pointer is NULL. No data in Stack\033[0m\n");
		printf("\033[32mReturning back...\033[0m\n\n");
		return 0;
	}
	printf("Stack [%p], \"%s\" from func: %s(%zu) file: %s\n",
		stk,
		stk->stack_creation_inf.var_name,
		stk->stack_creation_inf.file,
		stk->stack_creation_inf.line,
		stk->stack_creation_inf.file);

	PrintStackErrors(stk->errors);

	printf("{\n");
	printf("size = %zu\n", stk->size);
	printf("capacity = %zu\n", stk->capacity);
	if (!(stk->errors & STACK_DATA_NULL))
	{
		printf("data[%p]\n", stk->data);
		printf("  {\n");
		for (size_t i = 0; i < stk->capacity; i++)
		{
			PRINT_DATA(i, stk->data[i]);
			if (stk->data[i] == POISON_ELEM)
				printf(" ( POISON )");
			printf("\n");
		}
		printf("  }\n");
	}
	printf("}\n");

	printf("\033[32mReturning back...\033[0m\n\n");
	stk->errors = 0;
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