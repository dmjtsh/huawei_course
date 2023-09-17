#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>

#include "poem_sorter.h"


int main(int argc, char** argv)
{
	Poem poem = {0, 0, NULL, NULL};
	
	const char* file_name = NULL; 
	if (argc < 2)
		file_name = "poems/onegin.txt";
	else
		file_name = argv[1];

	initialize_poem(&poem, file_name);

	sort_poem(&poem, STRAIGHT);
	record_poem(&poem, "sorted_poem.txt");
	print_poem(&poem);

	sort_poem(&poem, REVERSE);
	record_poem(&poem, "reverse_sorted_poem.txt");
	print_poem(&poem);

	sort_poem(&poem, ORIGINAL);
	record_poem(&poem, "original_poem.txt");
	print_poem(&poem);

	destroy_poem(&poem);

	return 0;
}