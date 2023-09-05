#include <stdio.h>
#include <math.h>

const double EPSILON = 1e-3;

void clean_buffer()
{
	while (getchar() != '\n') { ; }
}

bool is_equal(double first_num, double second_num)
{
	return fabs(first_num - second_num) < EPSILON;
}

bool is_zero(double num)
{
	return is_equal(num, 0);
}