#ifndef STACK
#define STACK

#define STACK_DUMP(stk)               StackDump ((stk), __FILE__, __LINE__, __func__)
#define STACK_CTOR(stack)             StackCtor ((stack), #stack, __FILE__, __LINE__, __func__)
#define PRINT_DATA(i, stk_data_elem)  printf("  [%zu] = %d\n", i, stk_data_elem);

enum StackErrors { STACK_NULL = 1, STACK_DATA_NULL = 2, STACK_SIZE_GREATER_CAPACITY = 4,  STACK_SIZE_LESS_ONE = 8};

typedef int Elem_t;

struct StackCreationInf
{
	size_t line;
	const char* file;
	const char* func;
	const char* var_name;
};

struct Stack
{
	const long long left_canary = 0xAB0BA228;
	size_t size;
	size_t capacity;

	unsigned errors;
	StackCreationInf stack_creation_inf;

	Elem_t* data;
	const long long right_canary = 0xAB0BA228;
};

Elem_t StackPop(Stack* stk);

int StackPush(Stack* stk, Elem_t elem);

int StackDtor(Stack* stk);

int StackCtor(Stack* stk, const char* var_name, const char* file_name, size_t line, const char* func_name);

#endif