#include <stdio.h>
#include "stack.h"

void print_stack_elems(Stack* stk)
{
	for (size_t i = 0; i < stk->size; i++)
	{
		printf("elem[%zu] = %d\n", i, stk->data[i]);
	}
}