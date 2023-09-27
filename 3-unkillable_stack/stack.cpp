#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "io.h"

#ifdef _DEBUG
Hash_t StackHash(Stack* stk)
{
	stk->hash = 0;
	return Hash(stk, sizeof(Stack)) + Hash(stk->data, stk->size * sizeof(Elem_t));
}

int StackState(Stack* stk)
{
	if (!stk)
		return -1;

	if (stk->size > stk->capacity)                           stk->errors |= STACK_SIZE_GREATER_CAPACITY;
	if (stk->left_canary != STACK_CANARY_NUM)                stk->errors |= STACK_LCANARY_DMG;
	if ((stk->right_canary != STACK_CANARY_NUM))             stk->errors |= STACK_RCANARY_DMG;

	if (!stk->data)
	{ 
		stk->errors |= STACK_DATA_NULL;
		return -1;
	}
	if (((Canary_t*)stk->data)[-1] != STACK_DATA_CANARY_NUM) stk->errors |= STACK_DATA_LCANARY_DMG;
	if (stk->data[stk->capacity] != STACK_DATA_CANARY_NUM)   stk->errors |= STACK_DATA_RCANARY_DMG;

	Hash_t old_hash = stk->hash;
	Hash_t new_hash = StackHash(stk);
	if (old_hash != new_hash)                                stk->errors |= STACK_HASH_MISMATCH;
	else stk->hash = old_hash;

	if (stk->size >= STACK_MAX_SIZE)                         stk->errors |= STACK_BAD_SIZE;
	if (stk->capacity >= STACK_MAX_SIZE)                     stk->errors |= STACK_BAD_CAPACITY;

	if (stk->errors)
		return -1;

	return 0;
}

int StackStateBeforePop(Stack* stk)
{
	if (StackState(stk) && stk == NULL)
		return -1;

	if (stk->size < 1)
	{
		stk->errors |= STACK_SIZE_LESS_ONE;
		return -1;
	}

	return 0;
}
#endif

#ifdef _DEBUG
int StackDataRealloc(Stack* stk, size_t new_capacity)
{
	if (!stk)
		return -1;

	if (stk->data != NULL) // IF NOT FIRST CALL
		stk->data = (Elem_t*)((size_t)stk->data-sizeof(Canary_t));

	while ((new_capacity * sizeof(Elem_t)) % 8 != 0)  // MB DOESNT WORK
		new_capacity++;

	Elem_t* new_data = (Elem_t*)realloc(stk->data, new_capacity * sizeof(Elem_t) + 2 * sizeof(Canary_t));
	if (!new_data)
	{
		stk->errors |= STACK_REALLOC_ERROR;
		return -1; 
	}
	stk->data = new_data;
	stk->data = (Elem_t*)((size_t)stk->data + sizeof(Canary_t));
	
	// FILL WITH POISON
	Elem_t* tmp = (Elem_t*)((size_t)stk->data + sizeof(Elem_t) * stk->size);
	for (size_t i = 0; i < new_capacity - stk->size; i++)
		tmp[i] = POISON_ELEM;

	((Canary_t*)stk->data)[-1] = STACK_DATA_CANARY_NUM;
	stk->data[new_capacity] =    STACK_DATA_CANARY_NUM;

	stk->capacity = new_capacity;

	return 0;
}
#else
int StackDataRealloc(Stack* stk, size_t new_capacity)
{
	if (!stk)
		return -1;

	Elem_t* new_data = (Elem_t*)realloc(stk->data, new_capacity * sizeof(Elem_t));
	if (!new_data)
		return -1;
	stk->data = new_data;

	stk->capacity = new_capacity;

	return 0;
}
#endif

#ifdef _DEBUG
int StackPop(Stack* stk, Elem_t* deleted_elem)
{
	int stack_state = StackStateBeforePop(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state)
		return -1;

	Elem_t ret_value = stk->data[--stk->size];
	stk->data[stk->size] = POISON_ELEM;

	if (stk->capacity > (stk->size+1) * 4)
		StackDataRealloc(stk, stk->capacity / 4);

	stk->hash = StackHash(stk);

	stack_state = StackStateBeforePop(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state)
		return -1;

	*deleted_elem = ret_value;
	return 0;
}
#else
int StackPop(Stack* stk, Elem_t* deleted_elem)
{
	if (stk->size == 0)
		return -1;

	Elem_t ret_value = stk->data[--stk->size];
	stk->data[stk->size] = 0;

	if (stk->capacity > (stk->size + 1) * 4)
		if (StackDataRealloc(stk, stk->capacity / 4) == -1)
			return -1;

	*deleted_elem = ret_value;
	return 0;
}
#endif

#ifdef _DEBUG
int StackPush(Stack* stk, Elem_t elem)
{
	int stack_state = StackState(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state)
		return -1;

	if (stk->capacity <= stk->size)
		StackDataRealloc(stk, stk->capacity * 2);

	stk->data[stk->size++] = elem;
	stk->hash = StackHash(stk);

	stack_state = StackState(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state)
		return -1;

	return 0;
}
#else 
int StackPush(Stack* stk, Elem_t elem)
{
	if (stk->capacity <= stk->size)
		if (StackDataRealloc(stk, stk->capacity * 2) == -1)
			return -1;

	stk->data[stk->size++] = elem;

	return 0;
}
#endif

#ifdef _DEBUG
int STACK_CTOR(Stack* stk, const char* var_name, const char* file_name, size_t line, const char* func_name)
{
	if (!stk)
	{
		STACK_DUMP_TO_FILE(stk);
		return -1;
	}

	stk->stack_creation_inf.var_name = var_name;
	stk->stack_creation_inf.file     = file_name;
	stk->stack_creation_inf.line     = line;
	stk->stack_creation_inf.func     = func_name;

	stk->left_canary  = STACK_CANARY_NUM;
	stk->right_canary = STACK_CANARY_NUM;

	size_t start_capacity = 2;
	StackDataRealloc(stk, start_capacity);

	stk->size   = 0;
	stk->errors = 0;

	stk->hash = StackHash(stk);

	int stack_state = StackState(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state)
		return -1;

	return 0;
}
#else
int StackCtor(Stack* stk)
{
	if (!stk)
		return -1;

	size_t start_capacity = 2;
	if (StackDataRealloc(stk, start_capacity) == -1)
		return -1;

	stk->size = 0;

	return 0;
}
#endif

#ifdef _DEBUG
int StackDtor(Stack* stk)
{

	int stack_state = StackState(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state)
		return -1;

	stk->stack_creation_inf.line     = 0;
	stk->stack_creation_inf.file     = NULL;
	stk->stack_creation_inf.func     = NULL;
	stk->stack_creation_inf.var_name = NULL;

	stk->data = (Elem_t*)((size_t)stk->data - sizeof(Canary_t));
	free(stk->data);

	stk->capacity = 0;
	stk->size     = 0;
	stk->errors   = STACK_NULL + STACK_DATA_NULL;
	return 0;
}
#else
int StackDtor(Stack* stk)
{
	if (!stk || !stk->data)
		return -1;

	free(stk->data);

	stk->capacity = 0;
	stk->size = 0;

	return 0;
}
#endif