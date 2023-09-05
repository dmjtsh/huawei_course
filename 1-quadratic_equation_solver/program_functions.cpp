#include <stdio.h>
#include <math.h>

#include "constants.h"

void clean_buffer()
{
	while (getchar() != '\n') { ; }
}

bool is_equal(double first_num, double second_num)
{
	return fabs(first_num - second_num) < EPSILON;
}
