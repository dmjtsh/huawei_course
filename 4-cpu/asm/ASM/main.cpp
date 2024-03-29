#include <stdio.h>

#include "io.h"
#include "assembler.h"

int main(int argv, char* argc[])
{
	ASM assembler = {};
	if (argv <= 2)
		ASMCtor(&assembler, "../examples/bublic.txt", "test_compiled.bin");
	else
		ASMCtor(&assembler, argc[1], argc[2]);

	ASMProcessFile(&assembler);

	ASMDtor(&assembler);

	return 0;
}