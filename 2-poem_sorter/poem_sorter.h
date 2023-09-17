#ifndef POEM_SORTER
#define POEM_SORTER

#include "io.h"

enum SortOrders {ORIGINAL = 0, STRAIGHT = 1, REVERSE = -1 };
void sort_poem(Poem* poem, int sort_order);

#endif 