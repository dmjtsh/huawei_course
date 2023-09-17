#include <stdio.h>
#include <assert.h>

#include "poem.h"

void print_str(const char* str)
{
	assert(str != NULL);
	printf("%s\n", str);
}

void print_poem(Poem* poem)
{
	assert(poem != NULL);

	printf("\n");
	for (size_t counter_strings = 0; counter_strings <= poem->strings_num; counter_strings++)
	{
		print_str(poem->strings[counter_strings]);
	}
}