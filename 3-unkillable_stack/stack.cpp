#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "io.h"

int StackDump(Stack* stk, const char* file_name, const size_t line, const char* func_name)
{
	printf("\n\033[32mOoops, something wrong happend, that called StackDump\033[0m\n");
	printf("called from func: %s(%zu) file: %s\n",
		func_name,
		line,
		file_name);
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

	printf("{\n");

	if ((stk->errors & STACK_LCANARY_DMG))
		printf("\033[31mStack Left Canary DAMAGED\033[0m\n");
	if ((stk->errors & STACK_RCANARY_DMG))
		printf("\033[31mStack Right Canary DAMAGED\033[0m\n");
	if (stk->errors & STACK_SIZE_GREATER_CAPACITY)
		printf("\033[31mStack Size GREATER Capacity\033[0m\n");
	if (stk->errors & STACK_SIZE_LESS_ONE)
		printf("\033[31mStack Size Less One\033[0m\n");

	printf("size = %zu\n", stk->size);
	printf("capacity = %zu\n", stk->capacity);

	if (!(stk->errors & STACK_DATA_NULL))
	{
		if (stk->errors & STACK_DATA_LCANARY_DMG)
			printf("\033[31mStack DATA Left Canary DAMAGED\033[0m\n");
		if (stk->errors & STACK_DATA_RCANARY_DMG)
			printf("\033[31mStack DATA Right Canary DAMAGED\033[0m\n");

		printf("data[%p]\n", stk->data);
		printf("  {\n");
		for (size_t i = 0; i < stk->capacity; i++)
			PRINT_DATA(i, stk->data[i]);
		printf("  }\n");
	}
	else
		printf("\033[31mStack Data is NULL\033[0m\n");

	printf("}\n");
	printf("\033[32mReturning back...\033[0m\n\n");

	stk->errors = 0;
	return 1;
}

int StackOk(Stack* stk)
{
	if (!stk)
		return 0;

	if (stk->size > stk->capacity)
		stk->errors |= STACK_SIZE_GREATER_CAPACITY;
	if (stk->left_canary != STACK_CANARY_NUM)
		stk->errors |= STACK_LCANARY_DMG;
	if ((stk->right_canary != STACK_CANARY_NUM))
		stk->errors |= STACK_RCANARY_DMG;
	if (!stk->data)
	{
		stk->errors |= STACK_DATA_NULL;
		return 0;
	}
	if (((Canary_t*)stk->data)[-1] != STACK_DATA_CANARY_NUM)
		stk->errors |= STACK_DATA_LCANARY_DMG;
	if (stk->data[stk->capacity] != STACK_DATA_CANARY_NUM)
		stk->errors |= STACK_DATA_RCANARY_DMG;

	if (stk->errors)
		return 0;

	return 1;
}

int StackBeforePopOk(Stack* stk)
{
	if (!StackOk(stk))
		return 0;

	if (stk->size < 1)
	{
		stk->errors |= STACK_SIZE_LESS_ONE;
		return 0;
	}
	return 1;
}

int StackDataRealloc(Stack* stk, size_t new_capacity)
{
	if (!stk)
		return 0;

	if (stk->data != NULL)
		stk->data = (Elem_t*)((size_t)stk->data-sizeof(Canary_t));
	stk->data = (Elem_t*)realloc(stk->data, new_capacity * sizeof(Elem_t) + 2 * sizeof(Canary_t));

	stk->data = (Elem_t*)((size_t)stk->data + sizeof(Canary_t));

	((Canary_t*)stk->data)[-1] = STACK_DATA_CANARY_NUM;
	stk->data[new_capacity] =    STACK_DATA_CANARY_NUM;

	stk->capacity = new_capacity;

	return 1;
}

Elem_t StackPop(Stack* stk)
{
	if (!StackBeforePopOk(stk))
	{
		STACK_DUMP(stk);
		return 0;
	}

	Elem_t ret_value = stk->data[--stk->size];
	stk->data[stk->size] = 0;

	if (stk->capacity > (stk->size+1) * 4)
	{
		StackDataRealloc(stk, stk->capacity / 4); 

		if (!StackOk(stk))
		{
			STACK_DUMP(stk);
			return 0;
		}
	}

	if (!StackOk(stk))
	{
		STACK_DUMP(stk);
		return 0;
	}
	return ret_value;
}

int StackPush(Stack* stk, Elem_t elem)
{
	if (!StackOk(stk))
	{
		STACK_DUMP(stk);
		return 0;
	}

	if (stk->capacity <= stk->size)
	{ 
		StackDataRealloc(stk, stk->capacity * 2);

		if (!StackOk(stk))
		{
			STACK_DUMP(stk);
			return 0;
		}
	}

	stk->data[stk->size++] = elem;
	if (!StackOk(stk))
	{
		STACK_DUMP(stk);
		return 0;
	}
	return 0;
}
int StackDtor(Stack* stk)
{
	assert(stk != NULL);
	assert(stk->data != NULL);

	stk->stack_creation_inf.line =     0;
	stk->stack_creation_inf.file =     NULL;
	stk->stack_creation_inf.func =     NULL;
	stk->stack_creation_inf.var_name = NULL;

	stk->data = (Elem_t*)((size_t)stk->data - sizeof(Canary_t));
	free(stk->data);
	stk->capacity = 0;
	stk->size =     0;
	stk->errors = STACK_NULL + STACK_DATA_NULL + STACK_SIZE_LESS_ONE + STACK_SIZE_GREATER_CAPACITY;
	return 0;
}


int StackCtor(Stack* stk, const char* var_name, const char* file_name, size_t line, const char* func_name)
{
	assert(stk != NULL);

	stk->stack_creation_inf.var_name = var_name;
	stk->stack_creation_inf.file =     file_name;
	stk->stack_creation_inf.line =     line;
	stk->stack_creation_inf.func =     func_name;

	stk->left_canary =  STACK_CANARY_NUM;
	stk->right_canary = STACK_CANARY_NUM;

	size_t start_capacity = 2;
	StackDataRealloc(stk, start_capacity);

	stk->size =   0;
	stk->errors = 0;

	return 0;
}