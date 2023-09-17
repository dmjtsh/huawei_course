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

void record_poem(Poem* poem, const char* file_path)
{
	assert(poem != NULL);
	assert(file_path != NULL);

	FILE* file_to_record = NULL;
	fopen_s(&file_to_record, file_path, "w");
	assert(file_to_record != NULL);

	for (size_t counter_strings = 0; counter_strings <= poem->strings_num; counter_strings++)
	{
		fputs(poem->strings[counter_strings], file_to_record);
		fputs("\n", file_to_record);
	}

	fclose(file_to_record);
}