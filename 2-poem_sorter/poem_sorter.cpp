#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>

#include "utilities.h"
#include "io.h"

void strings_swap(char** str1, char** str2)
{
	assert(str1 != NULL);
	assert(str2 != NULL);

	char* tmp_p = *str1;
	*str1 = *str2;
	*str2 = tmp_p;
}

size_t partition(char* data[], size_t left, size_t right, void* comporator)
{
	assert(data != NULL);

	size_t r_index = right;
	size_t l_index = left;

	size_t pivot_index = borders_rand(left, right);
	
	char* pivot = data[pivot_index];
	assert(pivot != NULL);
	while (l_index < r_index)
	{
		while (strcmp(data[l_index], pivot) < 0 && l_index < r_index)
		{
			l_index++;
			assert(l_index <= right);
			assert(l_index >= left);
		}

		while (strcmp(data[r_index], pivot) > 0 && l_index < r_index)
		{
			r_index--;
			assert(r_index <= right);
			assert(r_index >= left);
		}

		if (l_index < r_index)
		{
			strings_swap(&data[l_index], &data[r_index]);
		}

		if (strcmp(data[l_index], data[r_index]) == 0)
			l_index++;


	}
	return r_index;
}


void quick_sort(char* data[], size_t left, size_t right, void* comporator)
{
	assert(data != NULL);

	if (right <= left)
		return;

	size_t mid = partition(data, left, right, comporator);

	quick_sort(data, mid + 1, right, comporator);
	if (mid != 0)
		quick_sort(data, left, mid - 1, comporator);

}

void sort_poem(Poem* poem)
{
	assert(poem != NULL);
	quick_sort(poem->strings, 0, poem->strings_num, strcmp);
}