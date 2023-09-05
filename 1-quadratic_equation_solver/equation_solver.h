struct EquationCoeffs
{
	double a;
	double b;
	double c;
};

struct EquationRoots
{
	double x1;
	double x2;
};

enum NumberOfRoots { INFINITE_ROOTS = -1, ZERO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2 };

int solve_quadratic_equation (EquationCoeffs* coeffs, EquationRoots* roots);