#include <stdio.h>

#include "io.h"
#include "cpu.h"


int main(int argv, char* argc[])
{
	CPU cpu = {};
	if (argv <= 1)
		CPUCtor(&cpu, "test.txt");
	else
		CPUCtor(&cpu, argc[1]);

	CPUProcessFile(&cpu);

	return 0;
}