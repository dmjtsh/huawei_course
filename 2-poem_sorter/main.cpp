#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>

#include "poem_sorter.h"


int main(int argc, char** argv)
{
	Poem poem = {NULL, NULL, NULL, NULL};
	
	const char* file_name = NULL; 
	if (argc < 2)
		file_name = "poems/onegin.txt";
	else
		file_name = argv[1];

	initialize_poem(&poem, file_name);
	print_poem(&poem, "init: "); // DEBUG
	
	printf("Array beginning: %p. Array ending: %p\n", poem.poem_strings->str, (char*)((size_t)poem.poem_strings->str + sizeof(PoemString) * poem.strings_num)); // DEBUG

	quick_sort((void*)(poem.poem_strings), 0, poem.strings_num - 1, poem_strings_ñomparator, sizeof(PoemString));
	print_poem(&poem, "after sort: "); // DEBUG
	record_poem(&poem, "sorted_poem.txt");

	quick_sort((void*)(poem.poem_strings), 0, poem.strings_num - 1, r_poem_strings_comparator, sizeof(PoemString));
	print_poem(&poem, "after reverse sort: "); // DEBUG
	record_poem(&poem, "reverse_sorted_poem.txt"); 

	destroy_poem(&poem);

	return 0;
}