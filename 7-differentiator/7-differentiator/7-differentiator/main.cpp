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

	char graph_name1[50] = "";
	char graph_name2[50] = "";

	for(size_t i = 2; i < 7; i++)
	{
		snprintf(graph_name1, sizeof(graph_name1), "Taylor Graph %d", i);
		snprintf(graph_name2, sizeof(graph_name2), "Taylor Diff Graph %d", i);

		PlotTaylorTreeGraph(&expr_tree, graph_name1, 'x', i, 0);
		PlotTaylorDiffGraph(&expr_tree, graph_name2, 'x', i, 0);
	}
	OptimizeTree(&expr_tree);

	TreeDtor(&expr_tree);

	IntermediatePrinterDtor(&INTERMEDIATE_PRINTER);
}

