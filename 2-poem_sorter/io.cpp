#include <stdio.h>
#include <assert.h>

#include "poem.h"

void print_str(char* const str)
{
	printf("%s\n", str);
}

void print_poem(Poem* poem)
{
	assert(poem != NULL);

	size_t counter_strings = 0;
	while (counter_strings < poem->strings_num)
	{
		print_str(poem->strings[counter_strings]);
		counter_strings++;
	}
}