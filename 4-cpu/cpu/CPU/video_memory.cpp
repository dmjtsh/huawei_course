#include "video_memory.h"

void DrawRAM(Elem_t* RAM)
{
	for(size_t i = 0; i < 10; i++)
	{
		for(size_t j = 0; j < 10; j++)
		{
			size_t current_index = i * 10 + j;
			if(RAM[current_index] != 0)
				printf("* ");
			else
				printf("- ");
		}
		printf("\n");
	}
}