#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>
#include "utilities.h"

#include "poem.h"

void swap(int* num1, int* num2)
{
	assert(num1 != NULL);
	assert(num2 != NULL);
	int tmp = *num1;
	*num1 = *num2;
	*num2 = tmp;
}

size_t partition(int data[], size_t left, size_t right)
{
	assert(data != NULL);

	size_t r_index = right;
	size_t l_index = left;

	size_t pivot_index = borders_rand(left, right);

	int pivot = data[pivot_index];
	while (l_index < r_index)
	{
		while (data[l_index] < pivot && l_index < r_index)
		{
			l_index++;
			assert(l_index <= right);
			assert(l_index >= left);
		}

		while (data[r_index] > pivot && l_index < r_index)
		{
			r_index--;
			assert(r_index <= right);
			assert(r_index >= left);
		}

		if (l_index < r_index)
		{
			swap(&data[l_index], &data[r_index]);
		}
		if (data[l_index] == data[r_index])
			l_index++;
	}
	return r_index;
}


void quick_sort(int data[], size_t left, size_t right)
{
	assert(data != NULL);

	if (right <= left)
		return;

	size_t mid = partition(data, left, right);

	quick_sort(data, mid + 1, right);
	if (mid != 0)
		quick_sort(data, left, mid - 1);


}

void sort_poem(Poem* poem)
{
	;
	// SORT PROCESS ... //
}