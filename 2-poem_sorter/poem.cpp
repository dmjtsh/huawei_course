#include <assert.h>
#include <cstdlib>

#include "io.h"
#include "utilities.h"

void inititalize_strings(Poem* poem)
{
	assert(poem->poem_text != NULL);
	assert(poem->strings_num != -1);
	assert(poem->poem_size != -1);

	poem->strings = (char**)calloc(poem->strings_num+1, sizeof(char*));
	assert(poem->strings != NULL);

	poem->strings[0] = poem->poem_text;
	size_t strings_counter = 0;

	char* start_p = poem->poem_text;
	for (char* current_p = start_p; current_p - start_p <= poem->poem_size; current_p++)
		if (*current_p == '\n')
		{
			*current_p = '\0';

			poem->strings[++strings_counter] = current_p + 1;
			assert(poem->strings[strings_counter] != NULL);
		}
}

void initialize_poem(Poem* poem, const char* file_name)
{
	assert(file_name != NULL);
	assert(poem != NULL);

	poem->poem_size = get_file_size(file_name);
	assert(poem->poem_size != -1);
	poem->poem_text = get_file_text(file_name);
	assert(poem->poem_text != NULL);
	
	poem->strings_num = count_text_strings(poem->poem_text);
	assert(poem->strings_num != -1);

	inititalize_strings(poem);
}

void destroy_poem(Poem* poem)
{
	assert(poem != NULL);

	free(poem->strings);
	free(poem->poem_text);
}