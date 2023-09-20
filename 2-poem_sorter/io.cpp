#include <stdio.h>
#include <assert.h>

#include "poem.h"

void print_str(const char* str)
{
	assert(str != NULL);

	printf("%s\n", str);
}

void print_poem(Poem* poem, const char* name)
{
	assert(poem != NULL);

	printf("\n");
	for (size_t strings_counter = 0; strings_counter < poem->strings_num; strings_counter++)
	{
		printf("%s", name);
		print_str(poem->poem_strings[strings_counter].str);
	}
}

void record_poem(Poem* poem, const char* file_path)
{
	assert(poem != NULL);
	assert(file_path != NULL);

	FILE* file_to_record = NULL;
	fopen_s(&file_to_record, file_path, "w");
	assert(file_to_record != NULL);

	for (size_t strings_counter = 0; strings_counter < poem->strings_num; strings_counter++)
	{
		fputs(poem->poem_strings[strings_counter].str, file_to_record);
		fputs("\n", file_to_record);
	}

	fclose(file_to_record);
}