#include <stdio.h>

#include "io.h"
#include "cpu.h"


int main(int argv, char* argc[])
{
	CPU cpu = {};
	if (argv <= 1)
		CPUCtor(&cpu, "test_compiled.bin");
	else
		CPUCtor(&cpu, argc[1]);

	CPUProcessFile(&cpu);

	return 0;
}