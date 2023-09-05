#include <stdio.h>
#include <assert.h>

#include "test.h"

/*
*
*	A little programm for solving quadratic equation.
*	Enter coeffs (a,b,c) for equation ax^2 + bx + c = 0.
*	Then you have your answer.
*	Dmit 2023.
*
*/

void input(EquationCoeffs* coeffs)
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

void answer(int number_of_roots, EquationRoots roots)
{
	switch (number_of_roots)
	{
	case INFINITE_ROOTS:
		printf("%s", "Infinite number of roots");
		break;
	case ZERO_ROOTS:
		printf("%s", "No roots");
		break;
	case ONE_ROOT:
		printf("x: %lf", roots.x1);
		break;
	case TWO_ROOTS:
		printf("x1: %lf x2: %lf", roots.x1, roots.x2);
		break;
	default:
		printf("Error, incorrect input");
		break;
	}
}

int main()
{
	EquationCoeffs coeffs;
	EquationRoots roots;

	run_tests(&roots);

	input(&coeffs);
	size_t number_of_roots = solve_equation(coeffs, &roots);
	answer(number_of_roots, roots);

	return 0;
}