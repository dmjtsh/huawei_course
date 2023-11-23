#include <stdio.h>

#include "io.h"
#include "cpu.h"


int main(int argv, char* argc[])
{
	CPU cpu = {};
	if (argv <= 1)
		CPUCtor(&cpu, "C:\\Users\\79370\\Desktop\\huawei_course\\4-cpu\\cpu\\CPU\\text_compiled.bin");
	else
		CPUCtor(&cpu, argc[1]);

	CPUProcessFile(&cpu);

	return 0;
}