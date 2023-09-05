#include <cstdlib>

#include "equation_solver.h"

void input_coeffs(EquationCoeffs* coeffs); 

void output_roots(int num_roots, EquationRoots* roots);

void output_test_result(int num_roots, EquationRoots* roots, int answer_num_roots, EquationRoots* answer_roots, size_t test_counter);