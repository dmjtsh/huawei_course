#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "utilities.h"
#include "poem_sorter.h"

int main()
{
	const char* file_path = "novels/onegin.txt";
	FILE* poem = NULL;
	fopen_s(&poem, file_path, "r");

	assert(poem != NULL);

	sort_poem(poem);
}