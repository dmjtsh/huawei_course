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
	EquationCoeffs coeffs = { NAN, NAN, NAN };
	EquationRoots   roots = { NAN, NAN };

	#ifdef _DEBUG
	run_tests();
	#endif

	input_coeffs(&coeffs);
	size_t number_of_roots = solve_quadratic_equation(&coeffs, &roots);
	output_roots(number_of_roots, &roots);
	
	system("pause");
	return 0;
}