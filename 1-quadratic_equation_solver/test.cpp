#include <assert.h>
#include <stdio.h>

#include "test.h"

void run_tests(EquationRoots* roots)
{
	FILE* tests_file = NULL;
	FILE* answers_file = NULL;
	fopen_s(&tests_file, "tests/test.txt", "r");
	fopen_s(&answers_file, "tests/answers.txt", "r");

	assert(tests_file != NULL);
	assert(answers_file != NULL);
	assert(roots != NULL);

	EquationCoeffs test_coeffs;
	EquationRoots answer_roots;
	int answer_num_roots = 0;

	while (fscanf_s(tests_file, "%lf %lf %lf", &test_coeffs.a, &test_coeffs.b, &test_coeffs.c) != EOF
		&& fscanf_s(answers_file, "%lf %lf %d", &answer_roots.x1, &answer_roots.x2, &answer_num_roots) != EOF)
	{
		int number_of_roots = solve_equation(test_coeffs, roots);
		if (number_of_roots == answer_num_roots)
		{
			switch (number_of_roots)
			{
			case INFINITE_ROOTS:
			case ZERO_ROOTS:
				printf("%s", "YES\n");
				break;
			case ONE_ROOT:
				if (is_equal(roots->x1, answer_roots.x1))
					printf("%s", "YES\n");
				else
					printf("%s", "No\n");
				break;
			case TWO_ROOTS:
				if (is_equal(roots->x1, answer_roots.x1) && is_equal(roots->x2, answer_roots.x2))
					printf("%s", "YES\n");
				else
					printf("%s", "NO\n");
				break;
			default:
				printf("%s", "ERROR\n");
			}
		}
		else
			printf("%s", "N1\n");
	}
}