#include <cstdlib>

#include "io.h"
#include "test.h"

/*
*
*	A little programm for solving quadratic equation.
*	Enter coeffs (a,b,c) for equation ax^2 + bx + c = 0.
*	Then you have your answer.
*	Dmit 2023.
*
*/

int main()
{
	EquationCoeffs coeffs;
	EquationRoots roots;

	coeffs.a = 0.0; coeffs.b = 0.0; coeffs.c = 0.0;
	roots.x1 = 0.0; roots.x2 = 0.0;

	#ifdef _DEBUG
	run_tests();
	#endif

	input_coeffs(&coeffs);
	size_t number_of_roots = solve_quadratic_equation(&coeffs, &roots);
	output_roots(number_of_roots, &roots);
	
	system("pause");
	return 0;
}