#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>
#include "utilities.h"

char** initialize_pointers(char* poem, size_t file_size)
{
	assert(poem != NULL);

	size_t strings_p_size = 2;
	char** strings_pointers = (char**)calloc(sizeof(char*), strings_p_size);
	strings_pointers[0] = poem;

	char* poem_start = poem;
	while (poem - poem_start <= file_size)
	{
		if (*poem == '\n')
		{
			*poem = '\0';
			strings_pointers[strings_p_size-1] = ++poem; //      ??????????????????????
			strings_pointers = (char**)realloc(strings_pointers, ++strings_p_size*sizeof(char)); //      ??????????????????????
			assert(strings_pointer != NULL);
		}
		else
			poem++;
	}

	return strings_pointers;
}

char* get_file_text(FILE* file, size_t file_size)
{
	assert(file != NULL);

	char* text = (char*)calloc(file_size, sizeof(char));
	fread(text, sizeof(char), file_size, file); //      ??????????????????????
	
	return text;
}

char** sort_poem(FILE* poem_file)
{
	assert(poem_file != NULL);

	size_t file_size = get_file_size(poem_file);
	char* poem = get_file_text(poem_file, file_size);
	assert(poem != NULL);

	char** strings_pointers = initialize_pointers(poem, file_size);
	
	// SORT PROCESS ... //

	return strings_pointers;
}