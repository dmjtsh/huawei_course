#include <stdio.h>

#include "io.h"

void run_tests(Stack* stk)
{
	printf("stack size: %zu\n", stk->size);
	printf("stack capacity: %zu\n", stk->capacity);

	StackPush(stk, 4);
	printf("stack size: %zu\n", stk->size);
	print_stack_elems(stk);

	printf("stack_pop: %d \n", StackPop(stk));
	printf("stack size: %zu\n", stk->size);
	printf("stack_pop: %d \n", StackPop(stk));

	StackPush(stk, 6);
	StackPush(stk, 5);
	StackPush(stk, 9);

	print_stack_elems(stk);
	printf("stack size: %zu\n", stk->size);
	printf("stack capacity %zu\n", stk->capacity);
}
int main()
{
	Stack stk = {};
	STACK_CTOR(&stk);

	run_tests(&stk);

	StackDtor(&stk);
	return 0;
}