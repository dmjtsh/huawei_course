#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>

#include "utilities.h"
#include "io.h"

enum SortOrders {ORIGINAL = 0, STRAIGHT = 1, REVERSE = -1};

void swap_p(void* obj1, void* obj2, size_t elem_size)
{
	assert(obj1 != NULL);
	assert(obj2 != NULL);

	// DEBUG
	printf("func{swap_p} OLD STR1: [%p] | string: %s | len: %d\n", obj1 ,((PoemString*)obj1)->str, ((PoemString*)obj1)->len);
	printf("func{swap_p} OLD STR2: [%p] | string: %s | len: %d\n", obj2, ((PoemString*)obj2)->str, ((PoemString*)obj2)->len);
	// DEBUG

	for (size_t i = 0; i < elem_size; i++)
	{
		char tmp = NULL;
		tmp = ((char*)obj1)[i];
		((char*)obj1)[i] = ((char*)obj2)[i];
		((char*)obj2)[i] = tmp;
	}

	// DEBUG
	printf("func{swap_p} NEW STR1: [%p] | string: %s | len: %d\n", obj1, ((PoemString*)obj1)->str, ((PoemString*)obj1)->len);
	printf("func{swap_p} NEW STR2: [%p] | string: %s | len: %d\n", obj2, ((PoemString*)obj2)->str, ((PoemString*)obj2)->len);
	// DEBUG
}

const char* skip_non_letters(const char* str)
{
	for (; !(*str >= 65 && *str <= 90) && !(*str >= 97 && *str <= 122) && *str != '\0'; str++) { ; }
	return str;
}



int poem_strings_сomparator(const void* str1, const void* str2) {
	assert(str1 != NULL);
	assert(str2 != NULL);

	// DEBUG
	char* end_of_str1 = ((PoemString*)str1)->str + ((PoemString*)str1)->len - 1;
	char* end_of_str2 = ((PoemString*)str2)->str + ((PoemString*)str2)->len - 1;

	printf("func{STRINGS_COMPARATOR} : STRING_1 : STR [%04zu] | len: %03zu | strlen: %03zu | string: '%s'\n",
		((PoemString*)str1)->str, ((PoemString*)str1)->len, strlen(((PoemString*)str1)->str), ((PoemString*)str1)->str);
	printf("func{STRINGS_COMPARATOR} : STRING_2 : STR [%04zu] | len: %03zu | strlen: %03zu | string: '%s'\n",
		((PoemString*)str2)->str, ((PoemString*)str2)->len, strlen(((PoemString*)str2)->str), ((PoemString*)str2)->str);


	assert(((PoemString*)str1)->len == strlen(((PoemString*)str1)->str));
	assert(((PoemString*)str2)->len == strlen(((PoemString*)str2)->str));
	// DEBUG

	return strcmp (skip_non_letters(((PoemString*)str1)->str), skip_non_letters(((PoemString*)str2)->str));
}

const char* r_skip_non_letters(const char* str, const char* end_of_str)
{
	for (; !(*end_of_str >= 65 && *end_of_str <= 90) && !(*end_of_str >= 97 && *end_of_str <= 122) && end_of_str != str; end_of_str--) { ; }
	return end_of_str;
}

int r_poem_strings_comparator(const void* str1, const void* str2)
{
	assert(str1 != NULL);
	assert(str2 != NULL);

	const char* end_of_str1 = ((PoemString*)str1)->str + ((PoemString*)str1)->len - 1;
	const char* end_of_str2 = ((PoemString*)str2)->str + ((PoemString*)str2)->len - 1;

	end_of_str1 = r_skip_non_letters(((PoemString*)str1)->str, end_of_str1);
	end_of_str2 = r_skip_non_letters(((PoemString*)str2)->str, end_of_str2);

	// DEBUG
	printf("func{R_STRINGS_COMPARATOR} : STRING_1 : STR [%04zu] | len: %03zu | strlen: %03zu | string: '%s'\n",
		((PoemString*)str1)->str, ((PoemString*)str1)->len, strlen(((PoemString*)str1)->str), ((PoemString*)str1)->str);
	printf("func{R_STRINGS_COMPARATOR} : STRING_2 : STR [%04zu] | len: %03zu | strlen: %03zu | string: '%s'\n",
		((PoemString*)str2)->str, ((PoemString*)str2)->len, strlen(((PoemString*)str2)->str), ((PoemString*)str2)->str);

	assert(((PoemString*)str1)->len == strlen(((PoemString*)str1)->str));
	assert(((PoemString*)str2)->len == strlen(((PoemString*)str2)->str));
	// DEBUG

	assert(end_of_str1 != NULL);
	assert(end_of_str2 != NULL);

	while (end_of_str1 != ((PoemString*)str1)->str && end_of_str2 != ((PoemString*)str2)->str)
	{
		if (*end_of_str1 > *end_of_str2)
			return 1;
		else if (*end_of_str1 < *end_of_str2)
			return -1;
		end_of_str1--;
		end_of_str2--;
	}
	if (end_of_str1 != ((PoemString*)str1)->str && end_of_str2 == ((PoemString*)str2)->str)
		return 1;
	if (end_of_str1 == ((PoemString*)str1)->str && end_of_str2 != ((PoemString*)str2)->str)
		return -1;
	return 0;
}

size_t partition(void* data, size_t left, size_t right, int comparator(const void* a, const void* b), size_t elem_size)
{
	assert(data != NULL);

	size_t r_index = right;
	size_t l_index = left;

	size_t pivot_index = borders_rand(left, right);
	
	void* pivot = calloc(1, elem_size);
	assert(pivot != NULL);
	memcpy(pivot, (void*)((size_t) data + pivot_index * elem_size), elem_size);

	assert(pivot != NULL);
	while (l_index < r_index)
	{
		while (l_index < r_index &&  comparator((void*)((size_t)data + l_index * elem_size), pivot) < 0)
		{
			l_index++;
			assert(l_index <= right);
			assert(l_index >= left);
		}

		while (l_index < r_index && comparator((void*)((size_t)data + r_index * elem_size), pivot) > 0 )
		{
			r_index--;
			assert(r_index <= right);
			assert(r_index >= left);
		}

		if (l_index < r_index)
			swap_p((void*)((size_t)data + l_index * elem_size), (void*)((size_t)data + r_index * elem_size), elem_size);
		else
			break;

		if (comparator((void*)((size_t)data + l_index * elem_size), (void*)((size_t)data + r_index * elem_size)) == 0)
			l_index++;
	}
	free(pivot);
	return r_index;
}


void quick_sort(void* data, size_t left, size_t right, int comparator(const void* a, const void* b), size_t elem_size)
{
	assert(data != NULL);

	if (right <= left)
		return;
	else if (right - left == 1)
	{
		//DEBUG
		fprintf(stderr, "func{QUICK_SORT} : STRING_1 : PointerSTR1 %p | ", (PoemString*)((size_t)data + left * elem_size));

		fprintf(stderr, "STR [%04zu] | len: %03zu | strlen: %03zu | string: '%s'\n",
			((PoemString*)((size_t)data + left * elem_size))->str,
			((PoemString*)((size_t)data + left * elem_size))->len,
			strlen(((PoemString*)((size_t)data + left * elem_size))->str),
			(((PoemString*)((size_t)data + left * elem_size))->str));

		fprintf(stderr, "func{QUICK_SORT} : STRING_2 : PointerSTR1 %p |", (PoemString*)((size_t)data + right * elem_size));

		fprintf(stderr, "STR [%04zu] | len: %03zu | strlen: %03zu | string: '%s'\n",
			((PoemString*)((size_t)data + right * elem_size))->str,
			((PoemString*)((size_t)data + right * elem_size))->len,
			strlen(((PoemString*)((size_t)data + right * elem_size))->str),
			(((PoemString*)((size_t)data + right * elem_size))->str));
		// DEBUG

		int compare_res = comparator((void*)((size_t)data + left * elem_size), (void*)((size_t)data + right * elem_size));
		if (compare_res > 0)
			swap_p((void*)((size_t)data + left * elem_size), (void*)((size_t)data + right * elem_size), elem_size);
		return;
	}
	size_t mid = partition(data, left, right, comparator, elem_size);

	quick_sort(data, mid + 1, right, comparator, elem_size);
	if (mid != 0)
		quick_sort(data, left, mid - 1, comparator, elem_size);

}