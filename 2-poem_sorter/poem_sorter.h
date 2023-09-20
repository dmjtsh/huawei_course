#ifndef POEM_SORTER
#define POEM_SORTER

#include "io.h"

enum SortOrders {ORIGINAL = 0, STRAIGHT = 1, REVERSE = -1 };

int poem_strings_comparator(const void* str1, const void* str2);

int r_poem_strings_comparator(const void* str1, const void* str2);

void quick_sort(void* data, size_t left, size_t right, int comparator(const void* a, const void* b), size_t elem_size);

#endif 