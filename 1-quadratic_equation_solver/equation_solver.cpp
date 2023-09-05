#include <assert.h>
#include <math.h>
#include "equation_solver.h"
#include "utilities.h"

int solve_linear_equation(EquationCoeffs* coeffs, EquationRoots* roots)
{
	assert(roots != NULL);

	double b = coeffs->b;
	double c = coeffs->c;

	if (is_zero(b) && is_zero(c))
		return INFINITE_ROOTS;
	else if (is_zero(b))
		return ZERO_ROOTS;
	else
	{
		if (is_zero(c))
		{
			roots->x1 = 0;
			return ONE_ROOT;
		}
		else
		{
			roots->x1 = -c / b;
			return ONE_ROOT;
		}
	}
}

int solve_quadratic_equation(EquationCoeffs* coeffs, EquationRoots* roots)
{
	assert(roots != NULL);

	double a = coeffs->a;
	double b = coeffs->b;
	double c = coeffs->c;

	if (is_zero(a))
		return solve_linear_equation(coeffs, roots);
	else
	{
		double D = b * b - 4 * a * c;
		double sqrtD = sqrt(D);
		if (is_zero(D))
		{
			roots->x1 = (-b) / (2 * a);
			return ONE_ROOT;
		}
		else if (D < 0)
		{
			return ZERO_ROOTS;
		}
		else
		{
			roots->x1 = (-b + sqrtD) / (2 * a);
			roots->x2 = (-b - sqrtD) / (2 * a);
			return TWO_ROOTS;
		}
	}
}
