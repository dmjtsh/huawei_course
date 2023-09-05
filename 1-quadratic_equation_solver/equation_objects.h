struct EquationCoeffs
{
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
};

struct EquationRoots
{
	double x1 = 0.0;
	double x2 = 0.0;
};

enum NumberOfRoots { INFINITE_ROOTS = -1, ZERO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2 };