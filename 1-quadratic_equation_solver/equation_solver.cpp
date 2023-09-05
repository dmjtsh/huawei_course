#include <assert.h>
#include <math.h>

#include "constants.h"
#include "program_functions.h"
#include "equation_structs.h"

int solve_linear_equation(EquationCoeffs coeffs, EquationRoots* roots)
{
	assert(roots != NULL);

	double b = coeffs.b;
	double c = coeffs.c;

	if (is_equal(b, 0) && is_equal(c, 0))
		return INFINITE_ROOTS;
	else if (is_equal(b, 0))
		return ZERO_ROOTS;
	else
	{
		if (is_equal(c, 0))
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

int solve_quadratic_equation(EquationCoeffs coeffs, EquationRoots* roots, double D)
{
	assert(roots != NULL);

	double a = coeffs.a;
	double b = coeffs.b;

	double sqrtD = sqrt(D);
	if (is_equal(D, 0))
	{
		roots->x1 = (-b) / (2 * a);
		return ONE_ROOT;
	}
	else
	{
		roots->x1 = (-b + sqrtD) / (2 * a);
		roots->x2 = (-b - sqrtD) / (2 * a);
		return TWO_ROOTS;
	}
}

int solve_equation(EquationCoeffs coeffs, EquationRoots* roots)
{
	assert(roots != NULL);

	double a = coeffs.a;
	double b = coeffs.b;
	double c = coeffs.c;

	double D = b * b - 4 * a * c;
	if (is_equal(a, 0))
		return solve_linear_equation(coeffs, roots);
	else
	{
		if (D >= 0)
			return solve_quadratic_equation(coeffs, roots, D);
		else
			return ZERO_ROOTS;
	}
}
