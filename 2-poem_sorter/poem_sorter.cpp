#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>

#include "utilities.h"
#include "io.h"

enum SortOrders {ORIGINAL = 0, STRAIGHT = 1, REVERSE = -1};

void swap_p(void** obj1, void** obj2)
{
	assert(obj1 != NULL);
	assert(obj2 != NULL);

	void* tmp_p = *obj1;
	*obj1 = *obj2;
	*obj2 = tmp_p;
}

int strings_ñomparator(const void* str1, const void* str2) {
	assert(str1 != NULL);
	assert(str2 != NULL);

	return strcmp((const char*)str1, (const char*)str2);
}
int reverse_strings_comparator(const void* str1, const void* str2)
{
	assert(str1 != NULL);
	assert(str2 != NULL);

	char* end_of_str1 = NULL;
	char* end_of_str2 = NULL;
	for (end_of_str1 = (char*)str1; *(end_of_str1+1) != '\0'; end_of_str1++) { ; }
	for (end_of_str2 = (char*)str2; *(end_of_str2+1) != '\0'; end_of_str2++) { ; }


	assert(end_of_str1 != NULL);
	assert(end_of_str2 != NULL);

	while (end_of_str1 != (char*)str1 && end_of_str2 != (char*)str2)
	{
		if (*end_of_str1 > *end_of_str2)
			return 1;
		else if (*end_of_str1 < *end_of_str2)
			return -1;
		end_of_str1--;
		end_of_str2--;
	}
	if (end_of_str1 != (char*)str1 && end_of_str2 == (char*)str2)
		return 1;
	if (end_of_str1 == (char*)str1 && end_of_str2 != (char*)str2)
		return -1;
	return 0;
}

size_t partition(void* data[], size_t left, size_t right, int comparator(const void* a, const void* b))
{
	assert(data != NULL);

	size_t r_index = right;
	size_t l_index = left;

	size_t pivot_index = borders_rand(left, right);
	
	void* pivot = data[pivot_index];
	assert(pivot != NULL);
	while (l_index < r_index)
	{
		while (comparator(data[l_index], pivot) < 0 && l_index < r_index)
		{
			l_index++;
			assert(l_index <= right);
			assert(l_index >= left);
		}

		while (comparator(data[r_index], pivot) > 0 && l_index < r_index)
		{
			r_index--;
			assert(r_index <= right);
			assert(r_index >= left);
		}

		if (l_index < r_index)
		{
			swap_p(&data[l_index], &data[r_index]);
		}

		if (comparator(data[l_index], data[r_index]) == 0)
			l_index++;


	}
	return r_index;
}


void quick_sort(void* data[], size_t left, size_t right, int comparator(const void* a, const void* b))
{
	assert(data != NULL);

	if (right <= left)
		return;

	size_t mid = partition(data, left, right, comparator);

	quick_sort(data, mid + 1, right, comparator);
	if (mid != 0)
		quick_sort(data, left, mid - 1, comparator);

}

void sort_poem(Poem* poem, int sort_order)
{
	assert(poem != NULL);
	if (sort_order == STRAIGHT)
		quick_sort((void**)(poem->strings), 0, poem->strings_num, strings_ñomparator);
	else if (sort_order == REVERSE)
		quick_sort((void**)(poem->strings), 0, poem->strings_num, reverse_strings_comparator);
	else if (sort_order == ORIGINAL)
	{
		size_t strings_counter = 0;

		char* start_p = poem->poem_text;
		for (char* current_p = start_p; current_p - start_p <= poem->poem_size; current_p++)
			if (*current_p == '\0')
			{
				poem->strings[++strings_counter] = current_p + 1;
				assert(poem->strings[strings_counter] != NULL);
			}
	}
}