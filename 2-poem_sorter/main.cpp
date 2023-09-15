#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>

#include "poem_sorter.h"


int main(int argc, char** argv)
{
	Poem poem = {0, 0, NULL, NULL};
	assert(argc <= 1);

	char file_name[] = "poems/onegin.txt";
	initialize_poem(&poem, file_name);//argv[1]);

	sort_poem(&poem);
	print_poem(&poem);

	destroy_poem(&poem);

	return 0;
}