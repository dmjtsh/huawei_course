#include <stdio.h>

#include "io.h"

void RunTests(Stack* stk)
{
	printf("stack size: %zu\n", stk->size);
	printf("stack capacity: %zu\n", stk->capacity);

	StackPush(stk, 4);
	printf("stack size: %zu\n", stk->size);
	PrintStackElems(stk);

	Elem_t elem = 0;
	StackPop(stk, &elem);
	StackPop(stk, &elem);
	printf("stack_pop: %d \n", elem);
	printf("stack size: %zu\n", stk->size);
	printf("stack_pop: %d \n", elem);

	StackPush(stk, 6);
	StackPush(stk, 5);
	StackPush(stk, 9);
	StackPush(stk, 8);
	StackPush(stk, 7);

	PrintStackElems(stk);
	printf("stack size: %zu\n", stk->size);
	printf("stack capacity %zu\n", stk->capacity);
}

int main()
{
	Stack stk = {};
	StackCtor(&stk);

	RunTests(&stk);

	StackDtor(&stk);
	return 0;
}