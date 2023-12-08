#define _CRT_SECURE_NO_WARNINGS

#include "read_node.h"
#include "differentiator.h"
#include "differentiator_io.h"
#include "optimization.h"

extern IntermediatePrinter INTERMEDIATE_PRINTER;

int main()
{
	IntermediatePrinterCtor(&INTERMEDIATE_PRINTER);

	Tree expr_tree = {};
	TreeCtor(&expr_tree, DifferentiatorElemCtor, DifferentiatorElemDtor, DifferentiatorElemPrinter);
	InitializeExprTree(&expr_tree, "source.txt");

	expr_tree = GetDifferentiatedTree(expr_tree, 'x');

	OptimizeTree(&expr_tree);

	PlotTreeGraph(&expr_tree, "Graph");
	PlotTaylorTreeGraph(&expr_tree, "Taylor Graph", 'x', 3, 0);
	PlotTaylorDiffGraph(&expr_tree, "Taylor Diff Graph", 'x', 3, 0);

	OptimizeTree(&expr_tree);

	TreeDtor(&expr_tree);

	IntermediatePrinterDtor(&INTERMEDIATE_PRINTER);
}

