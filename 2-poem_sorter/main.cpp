#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>

#include "utilities.h"
#include "poem_sorter.h"
#include "io.h"


int main(int argc, char** argv)
{
	Poem poem = {NAN, NAN, NULL, NULL};

//	initialize_poem(&poem, argv);

	sort_poem(&poem);

	destroy_poem(&poem);
	return 0;
}