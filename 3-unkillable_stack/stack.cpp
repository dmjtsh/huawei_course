#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "io.h"

// COMMON FUNCTIONS
int StackDataRealloc(Stack* stk, size_t new_capacity);

enum CallingFunc
{
	PUSH_FUNC = 0,
	POP_FUNC = 1
};
int StackDataReallocIfNeeded(Stack* stk, int calling_func)
{
	assert(stk != NULL);
	assert(stk->data != NULL);

	const int POP_COEFF = 4;
	const int PUSH_COEFF = 2;
	if (calling_func == PUSH_FUNC)
	{
		if (stk->capacity <= stk->size)
			return StackDataRealloc(stk, stk->capacity * PUSH_COEFF);
	}
	else if (calling_func == POP_FUNC)
	{
		if (stk->capacity > (stk->size + 1) * POP_COEFF)
			return StackDataRealloc(stk, stk->capacity / POP_COEFF);
	}

	return 0;
}

int StackPushKernal(Stack* stk, Elem_t elem)
{
	assert(stk != NULL);
	assert(stk->data != NULL);

	int realloc_res = StackDataReallocIfNeeded(stk, PUSH_FUNC);

	stk->data[stk->size] = elem;
	stk->size++;

	return realloc_res;
}

int StackPopKernal(Stack* stk, Elem_t* deleted_elem)
{
	assert(stk != NULL);
	assert(stk->data != NULL);

	stk->size--;
	Elem_t ret_value = stk->data[stk->size];

	int realloc_res = StackDataReallocIfNeeded(stk, POP_FUNC);

	*deleted_elem = ret_value;

	return realloc_res;
}

int StackCtorKernal(Stack* stk)
{
	assert(stk != NULL);

	const size_t START_CAPACITY = 2;

	stk->data = NULL;
	stk->size = 0;
	stk->capacity = 0;

	int realloc_res = StackDataRealloc(stk, START_CAPACITY);

	return realloc_res;
}

int StackDtorKernal(Stack* stk)
{
	assert(stk != NULL);
	assert(stk->data != NULL);

	free(stk->data);

	stk->capacity = 0;
	stk->size = 0;

	return 0;
}

// DEBUG FUNCTIONS
#ifdef _DEBUG

Hash_t StackHash(Stack* stk)
{
	if (!stk)
	{
		STACK_DUMP_TO_FILE(stk);
		return STACK_POINTER_NULL;
	}

	Hash_t old_hash = stk->hash;
	unsigned errors = stk->errors;

	stk->hash   = 0;
	stk->errors = 0;
	Hash_t new_hash = Hash(stk, sizeof(Stack)) + Hash(stk->data, stk->size * sizeof(Elem_t));
	
	stk->hash = old_hash;
	stk->errors = errors;

	return new_hash;
}

int StackSetError(unsigned* error, int error_bit)
{
	*error |= error_bit;
	return 0;
}

int StackUnsetError(unsigned* error, int error_bit)
{
	*error &= ~error_bit;
	return 0;
}

#define STACK_CHECK_ERROR(stk, condition, error) \
	if(condition)\
		StackSetError(&stk->errors, error);\
	else\
		StackUnsetError(&stk->errors, error);

int StackCheckState(Stack* stk)
{
	if (!stk)
		return STACK_POINTER_NULL;

	StackUnsetError(&stk->errors, STACK_SIZE_LESS_ONE); // NO NEED TO FIX THIS MISTAKE IF ITS NOT CALLED FROM POP

	STACK_CHECK_ERROR(stk, stk->size > stk->capacity, STACK_SIZE_GREATER_CAPACITY)
	STACK_CHECK_ERROR(stk, stk->left_canary  != STACK_CANARY_NUM, STACK_LCANARY_DMG)
	STACK_CHECK_ERROR(stk, stk->right_canary != STACK_CANARY_NUM, STACK_RCANARY_DMG)

	if (!stk->data) 
	{
		StackSetError(&stk->errors, STACK_DATA_POINTER_NULL);
		return stk->errors;
	}
	else 
		StackUnsetError(&stk->errors, STACK_DATA_POINTER_NULL);

	Canary_t* data_lcanary_p = (Canary_t*)((size_t)stk->data - sizeof(Canary_t));
	Canary_t* data_rcanary_p = (Canary_t*)((size_t)stk->data + stk->capacity * sizeof(Elem_t));
	STACK_CHECK_ERROR(stk, *data_lcanary_p != STACK_DATA_CANARY_NUM, STACK_DATA_LCANARY_DMG)
	STACK_CHECK_ERROR(stk, *data_rcanary_p != STACK_DATA_CANARY_NUM, STACK_DATA_RCANARY_DMG)

	Hash_t old_hash = stk->hash;
	Hash_t new_hash = StackHash(stk);
	STACK_CHECK_ERROR(stk, old_hash != new_hash, STACK_HASH_MISMATCH)

	STACK_CHECK_ERROR(stk, stk->size     >= STACK_MAX_SIZE, STACK_BAD_SIZE)
	STACK_CHECK_ERROR(stk, stk->capacity >= STACK_MAX_SIZE, STACK_BAD_CAPACITY)

	return stk->errors;
}	


int StackCheckStateBeforePop(Stack* stk)
{
	if (StackCheckState(stk) && stk == NULL)
		return STACK_POINTER_NULL;

	if (stk->size < 1)
		StackSetError(&stk->errors, STACK_SIZE_LESS_ONE);

	return stk->errors;
}

int StackFillPoison(Stack* stk, size_t capacity)
{
	if (!stk)
	{
		STACK_DUMP_TO_FILE(stk);
		return STACK_POINTER_NULL;
	}

	Elem_t* tmp = (Elem_t*)((size_t)stk->data + sizeof(Elem_t) * stk->size);
	for (size_t i = 0; i < capacity - stk->size; i++)
		tmp[i] = POISON_ELEM;

	return 0;
}

int StackDataRealloc(Stack* stk, size_t new_capacity)
{
	if (!stk)
	{
		STACK_DUMP_TO_FILE(stk);
		return STACK_POINTER_NULL;
	}

	if (stk->data != NULL) // IF NOT FIRST CALL
		stk->data = (Elem_t*)((size_t)stk->data-sizeof(Canary_t));

	while ((new_capacity * sizeof(Elem_t)) % sizeof(Canary_t) != 0) 
		new_capacity++;

	Elem_t* new_data = (Elem_t*)realloc(stk->data, new_capacity * sizeof(Elem_t) + 2 * sizeof(Canary_t));
	
	if (!new_data)
	{
		stk->errors |= STACK_REALLOC_ERROR;
		return stk->errors; 
	}

	stk->data = new_data;
	stk->data = (Elem_t*)((size_t)stk->data + sizeof(Canary_t));
	
	StackFillPoison(stk, new_capacity);

	Canary_t* data_lcanary_p = (Canary_t*)((size_t)stk->data - sizeof(Canary_t));
	Canary_t* data_rcanary_p = (Canary_t*)((size_t)stk->data + new_capacity * sizeof(Elem_t));
	*data_lcanary_p = STACK_DATA_CANARY_NUM;
    *data_rcanary_p = STACK_DATA_CANARY_NUM;

	stk->capacity = new_capacity;

	return 0;
}

int StackPush(Stack* stk, Elem_t elem)
{
	int stack_state = StackCheckState(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state && stk == NULL)
		return STACK_POINTER_NULL;
	else if (stack_state)
		return stk->errors;

	StackPushKernal(stk, elem);

	stk->hash = StackHash(stk);

	stack_state = StackCheckStateBeforePop(stk);
	STACK_DUMP_TO_FILE(stk);

	return stack_state;
}

int StackPop(Stack* stk, Elem_t* deleted_elem)
{
	int stack_state = StackCheckStateBeforePop(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state && stk == NULL)
		return STACK_POINTER_NULL;
	else if (stack_state)
		return stk->errors;

	StackPopKernal(stk, deleted_elem);

	stk->data[stk->size] = POISON_ELEM;
	stk->hash = StackHash(stk);

	stack_state = StackCheckState(stk);
	STACK_DUMP_TO_FILE(stk);

	return stack_state;
}

int _StackCtor(Stack* stk, const char* var_name, const char* file_name, size_t line, const char* func_name)
{
	if (!stk)
	{
		STACK_DUMP_TO_FILE(stk);
		return STACK_POINTER_NULL;
	}

	stk->stack_creation_inf.var_name = var_name;
	stk->stack_creation_inf.line = line;
	stk->stack_creation_inf.func = func_name;

	stk->left_canary  = STACK_CANARY_NUM;
	stk->right_canary = STACK_CANARY_NUM;

	StackCtorKernal(stk);
	stk->errors = 0;

	stk->hash = StackHash(stk);

	int stack_state = StackCheckState(stk);
	STACK_DUMP_TO_FILE(stk);

	return stack_state;
}

int StackDtor(Stack* stk)
{
	int stack_state = StackCheckState(stk);
	STACK_DUMP_TO_FILE(stk);
	if (stack_state && stk == NULL)
		return STACK_POINTER_NULL;
	else if (stack_state)
		return stk->errors;

	stk->stack_creation_inf.line = 0;
	stk->stack_creation_inf.func = NULL;
	stk->stack_creation_inf.var_name = NULL;

	stk->data = (Elem_t*)((size_t)stk->data - sizeof(Canary_t));
	stk->errors = STACK_POINTER_NULL | STACK_DATA_POINTER_NULL;

	return StackDtorKernal(stk);
}

// RELEASE FUNCTIONS
#else
int StackDataRealloc(Stack* stk, size_t new_capacity)
{
	assert(stk != NULL);

	Elem_t* new_data = (Elem_t*)realloc(stk->data, new_capacity * sizeof(Elem_t));
	if (!new_data)
		return STK_ERROR;

	stk->data = new_data;
	stk->capacity = new_capacity;

	return 0;
}

int StackPush(Stack* stk, Elem_t elem)
{
	if (!stk || !stk->data)
		return STK_ERROR;

	return StackPushKernal(stk, elem);
}

int StackPop(Stack* stk, Elem_t* deleted_elem)
{
	if (!stk || !stk->size || stk->size == 0)
		return STK_ERROR;

	int stack_pop_kernal_res = StackPopKernal(stk, deleted_elem);

	stk->data[stk->size] = 0;

	return stack_pop_kernal_res;
}


int StackCtor(Stack* stk)
{
	if (!stk || !stk->data)
		return STK_ERROR;

	return StackCtorKernal(stk);
}


int StackDtor(Stack* stk)
{
	if (!stk || !stk->data)
		return STK_ERROR;

	return StackDtorKernal(stk);
}

#endif