#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utilities.h"

size_t count_indents(char* buf_text)
{
	;
}

char* initialize_pointers(char* buf_text)
{
	;
}

void sort_poem(FILE* poem)
{
	assert(poem != NULL);

	size_t file_size = get_file_size(poem);

	size_t indents_num;

	char* buf_text = (char*)calloc(file_size, sizeof(char));
	fread(buf_text, sizeof(char), file_size, poem);
	indents_num = count_indents(buf_text);
	char* strings_pointers = initialize_pointers(buf_text);
}