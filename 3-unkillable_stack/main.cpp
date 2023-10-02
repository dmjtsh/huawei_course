#include <stdio.h>

#include "io.h"

void RunTests(Stack* stk)
{
	StackPush(stk, 4);

	Elem_t elem = 0;
	StackPop(stk, &elem);
	StackPop(stk, &elem);

	stk->data = NULL;
	StackPush(stk, 6);
	StackPush(stk, 5);
	StackPush(stk, 9);
	StackPush(stk, 8);
	StackPush(stk, 7);
}

int main()
{
	Stack stk = {};
	StackCtor(&stk);

	RunTests(&stk);

	StackDtor(&stk);
	return 0;
}