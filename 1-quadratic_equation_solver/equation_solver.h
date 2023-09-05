#pragma once
#include "equation_structs.h"

int solve_linear_equation (EquationCoeffs coeffs, EquationRoots* roots);

int solve_quadratic_equation (EquationCoeffs coeffs, EquationRoots* roots, double D);

int solve_equation (EquationCoeffs coeffs, EquationRoots* roots);