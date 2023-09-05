#include <stdio.h>
#include <assert.h>
#include <cstdlib>

#include "equation_solver.h"
#include "utilities.h"

void input_coeffs(EquationCoeffs* coeffs)
{
	assert(coeffs != NULL);

	printf("%s", "Dimas quadratic equation solver\n");
	size_t right_inputs = 0;
	do
	{
		printf("%s", "Enter coeffs(a, b, c) :\n");
		right_inputs = scanf_s("%lf %lf %lf", &coeffs->a, &coeffs->b, &coeffs->c);
		if (right_inputs != 3)
			clean_buffer();
	} while (right_inputs != 3);

}

void output_roots(int num_roots, EquationRoots* roots)
{
	assert(roots != NULL);

	switch (num_roots)
	{
	case INFINITE_ROOTS:
		printf("%s", "Infinite number of roots\n");
		break;
	case ZERO_ROOTS:
		printf("%s", "No roots\n");
		break;
	case ONE_ROOT:
		printf("x: %lf\n", roots->x1);
		break;
	case TWO_ROOTS:
		printf("x1: %lf x2: %lf\n", roots->x1, roots->x2);
		break;
	default:
		printf("Error, incorrect input\n");
		break;
	}
}

void output_test_result(int test_num_roots, EquationRoots* test_roots, int answer_num_roots, EquationRoots* answer_roots, size_t test_counter)
{
	assert(test_roots != NULL);
	assert(answer_roots != NULL);

	if (test_num_roots == answer_num_roots)
	{
		switch (test_num_roots)
		{
		case INFINITE_ROOTS:
		case ZERO_ROOTS:
			break;
		case ONE_ROOT:
			if (!is_equal(test_roots->x1, answer_roots->x1))
			{
				printf("-----------------\n");
				printf("Test number %zu\nWrong root value.\nNum of roots: 1.\n", test_counter);
				printf("received x0 = %lf, answer x0 = %lf\n", test_roots->x1, answer_roots->x1);
				printf("-----------------\n");
			}
			break;
		case TWO_ROOTS:
			if (!is_equal(test_roots->x1, answer_roots->x1) || !is_equal(test_roots->x2, answer_roots->x2))
			{
				printf("-----------------\n");
				printf("Test number %zu\nWrong roots values.\nNum of roots: 2.\n", test_counter);
				printf("received x1 = %lf, answer x1 = %lf\n", test_roots->x1, answer_roots->x1);
				printf("recieved x2 = %lf, answer x2 = %lf\n", test_roots->x2, answer_roots->x2);
				printf("-----------------\n");
			}
			break;
		default:
			printf("-----------------\n");
			printf("Test number %zu\nERROR.\nSomething wrong happend\n", test_counter);
			printf("-----------------\n");
		}
	}
	else
	{
		printf("-----------------\n");
		printf("Test number %zu\nWrong number of roots\n", test_counter);
		printf("-----------------\n");
	}
}