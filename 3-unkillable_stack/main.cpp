#include <stdio.h>

#include "stack.h"
#include "io.h"

extern FILE* g_log_file;
extern const char* g_log_file_path;

void RunTests(Stack* stk)
{
	StackPush(stk, 4);

	Elem_t elem = 0;
	StackPop(stk, &elem);

	StackPush(stk, 6);
	StackPush(stk, 5);
	StackPush(stk, 9);
	StackPush(stk, 8);
	StackPush(stk, 7);
}

int main()
{
	LogFileCtor(&g_log_file, g_log_file_path);
	Stack stk = {};
	StackCtor(&stk);

	RunTests(&stk);

	StackDtor(&stk);

	LogFileDtor(g_log_file);
	return 0;
}
