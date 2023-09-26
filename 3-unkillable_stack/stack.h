#ifndef STACK_H
#define STACK_H

typedef int Elem_t;
#define ELEM_T_SPECIFIER "%d"

#ifdef _DEBUG

#include "hash.h"

#define StackCtor(stack) STACK_CTOR ((stack), #stack, __FILE__, __LINE__, __func__)

typedef unsigned long long Canary_t;
const Elem_t POISON_ELEM = 666;

enum StackErrorBits {
	STACK_NULL                   = 1 << 0,
	STACK_DATA_NULL              = 1 << 1,
	STACK_SIZE_GREATER_CAPACITY  = 1 << 2,
	STACK_SIZE_LESS_ONE          = 1 << 3,
	STACK_LCANARY_DMG            = 1 << 4,
	STACK_RCANARY_DMG            = 1 << 5,
	STACK_DATA_LCANARY_DMG       = 1 << 5,
	STACK_DATA_RCANARY_DMG       = 1 << 6,
	STACK_HASH_MISMATCH          = 1 << 7,
	STACK_REALLOC_ERROR          = 1 << 8,
	STACK_BAD_CAPACITY           = 1 << 9,
	STACK_BAD_SIZE               = 1 << 10
};

enum CanaryConstants { STACK_CANARY_NUM = 0xAB0BA228, STACK_DATA_CANARY_NUM = 0xAB0BA322 };

const size_t STACK_MAX_SIZE = 10e30;
const size_t STACK_MAX_CAPACITY = 10e30;

struct StackCreationInfo
{
	size_t      line;
	const char* file;
	const char* func;
	const char* var_name;
};
struct Stack
{
	Canary_t left_canary;
	
	size_t size;
	size_t capacity;

	unsigned errors;
	StackCreationInfo stack_creation_inf;

	Elem_t* data;
	Hash_t  hash;

	Canary_t right_canary;
};

Hash_t StackHash(const Stack* stk);

#else
struct Stack
{
	size_t size;
	size_t capacity;

	Elem_t* data;
};
#endif

int StackPop(Stack* stk, Elem_t* deleted_elem);

int StackPush(Stack* stk, Elem_t elem);

int StackDtor(Stack* stk);

#ifdef _DEBUG
int STACK_CTOR(Stack* stk, const char* var_name, const char* file_name, size_t line, const char* func_name);
#else
int StackCtor(Stack* stk);
#endif

#endif