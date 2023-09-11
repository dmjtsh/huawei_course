#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "poem_sorter.h"
#include "io.h"

int main()
{
	const char* file_path = "poems/onegin.txt";
	FILE* poem_file = NULL;
	fopen_s(&poem_file, file_path, "r");

	assert(poem_file != NULL);

	char** sorted_poem_pointers = sort_poem(poem_file);

	print_poem(sorted_poem_pointers);
}