#include <assert.h>
#include <stdio.h>
#include <cstdlib>

#include "utilities.h"
#include "io.h"

void run_tests()
{
	FILE* tests_file = NULL;
	fopen_s(&tests_file, "tests/tests.txt", "r");

	assert(tests_file != NULL);


	EquationRoots test_roots;
	EquationRoots answer_roots;
	EquationCoeffs test_coeffs;

	test_roots.x1 = 0.0; test_roots.x2 = 0.0;
	answer_roots.x1 = 0.0; answer_roots.x2 = 0.0;
	test_coeffs.a = 0.0; test_coeffs.b = 0.0; test_coeffs.c = 0.0;

	int answer_num_roots = 0;
	
	size_t test_counter = 1;
	size_t mistakes_number = 0;
	while (fscanf_s(tests_file, "%lf %lf %lf %lf %lf %d", &test_coeffs.a, &test_coeffs.b, &test_coeffs.c,
		&answer_roots.x1, &answer_roots.x2, &answer_num_roots) != EOF)
	{
			int test_num_roots = solve_quadratic_equation(&test_coeffs, &test_roots);
			output_test_result(test_num_roots, &test_roots, answer_num_roots, &answer_roots, test_counter);
			test_counter++;
	}
	printf("%s", "Tests finished!\n");
}