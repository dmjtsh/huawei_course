#include <assert.h>
#include <cstdlib>

#include "poem.h"
#include "utilities.h"

void initialize_pointers(char* strings_pointers[], int strings_num, char* poem_text, int poem_size)
{
	assert(poem_text != NULL);
	assert(strings_num != -1);
	assert(poem_size != -1);

	char** strings_pointers = (char**)calloc(strings_num, sizeof(char*));
	assert(strings_pointers != NULL);

	strings_pointers[0] = poem_text;
	size_t strings_counter = 1;

	char* start_ch = poem_text;
	char* current_ch = poem_text;
	while (current_ch - start_ch <= poem_size)
	{
		if (*current_ch == '\n')
		{
			*current_ch = '\0';
			current_ch++;
			strings_pointers[strings_counter] = current_ch;

			assert(strings_pointers[strings_counter] != NULL);

			strings_counter++;
		}
		else
			current_ch++;
	}
}

void initialize_poem(Poem* poem, char* file_name)
{
	assert(file_name != NULL);
	assert(poem != NULL);

	poem->poem_size = get_file_size(file_name);
	assert(poem->poem_size != -1);
	poem->poem_text = get_file_text(file_name);
	assert(poem->poem_text != NULL);

	int strings_num = count_text_indents(poem->poem_text);
	assert(strings_num != -1);

	initialize_pointers(poem->strings, strings_num, poem->poem_text, poem->poem_size);
}

void destroy_poem(Poem* poem)
{
	assert(poem != NULL);

	free(poem->poem_text);
	for (size_t i = 0; i < poem->strings_num; i++)
	{
		free(poem->strings[i]);
	}

	free(poem->strings);
}