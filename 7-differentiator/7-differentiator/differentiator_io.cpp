#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdlib.h>

#include "differentiator.h"
#include "optimization.h"
#include "differentiator_io.h"

extern IntermediatePrinter INTERMEDIATE_PRINTER;

bool IsBracketsNeeded(TreeElemType node_type, OperPriority parent_node_priority, OperPriority child_node_priority)
{
	return node_type == OPER && parent_node_priority > child_node_priority;
}

// TODO: do tree verifier
void PrintLatexNode(const Tree* expr_tree, const TreeNode* node, const TreeNode* parent_node)
{
	assert(expr_tree != nullptr);

	if(node == nullptr)
		return;

	if (NODE_TYPE == NUM || NODE_TYPE == VAR)
	{
		DO_LATEX_PRINTF("%s", expr_tree->ElemPrinter(&node->node_elem));
		return;
	}
	assert(NODE_TYPE == OPER);

	OperPriority node_priority        = GetOperPriority(NODE_OPER);
	OperPriority parent_node_priority = LAST_PRIORITY;
	if (parent_node)
		parent_node_priority = GetOperPriority(parent_node->node_elem.elem.oper);

	if (IsBracketsNeeded(NODE_TYPE, parent_node_priority, node_priority))
		DO_LATEX_PRINTF("{");

	switch(NODE_OPER)
	{
	
		#define OPER_DEF(value, designation, priority, operation_num_args, latex_printing_code, ...)  \
		case value:                                                                                   \
			latex_printing_code;                                                                      \
			break;

		#include "opers_defs.h"

		default:
			assert(0);
	}
	
	#undef OPER_DEF

	if (IsBracketsNeeded(NODE_TYPE, parent_node_priority, node_priority))
		DO_LATEX_PRINTF("}");
}

// TODO: проверить чтоб файлики всегда в конце подавались или наборот в начале
void DoIntermidiatePrint(Tree* expr_tree, const TreeNode* node)
{
	assert(expr_tree != nullptr);
	assert(node      != nullptr);

	TextString* intermediate_print_text_strs = INTERMEDIATE_PRINTER.prints_text_info.text_strings;
	size_t current_str_num                   = INTERMEDIATE_PRINTER.current_str_num;
	size_t strings_num                       = INTERMEDIATE_PRINTER.prints_text_info.strings_num;

	char* current_str = intermediate_print_text_strs[current_str_num].str;  
	
	DO_LATEX_PRINTF("%s: ", current_str);
	
	DO_LATEX_PRINTF("$");
	PrintLatexNode(expr_tree, node, nullptr);
	DO_LATEX_PRINTF("$");

	if(current_str_num >= strings_num)
		current_str_num = 0;
	else
		current_str_num++;

	DO_LATEX_PRINTF(" ");

	INTERMEDIATE_PRINTER.current_str_num = current_str_num;
}

void PrintNode(const Tree* expr_tree, const TreeNode* node, const TreeNode* parent_node, FILE* file_to_print)
{
	assert(expr_tree != nullptr);

	if(node == nullptr)
		return;

	if (NODE_TYPE == NUM || NODE_TYPE == VAR)
	{
		fprintf(file_to_print, "%s", expr_tree->ElemPrinter(&node->node_elem));
		return;
	}
	assert(NODE_TYPE == OPER);

	OperPriority node_priority        = GetOperPriority(NODE_OPER);
	OperPriority parent_node_priority = LAST_PRIORITY;
	if (parent_node)
		parent_node_priority = GetOperPriority(parent_node->node_elem.elem.oper);

	if (IsBracketsNeeded(NODE_TYPE, parent_node_priority, node_priority))
		fprintf(file_to_print, "(");

	switch(NODE_OPER)
	{
		#define OPER_DEF(value, designation, priority, operation_num_args, latex_printing_code, ...) \
		case value:                                                                                  \
			PrintNode(expr_tree, node->left, node, file_to_print);								     \
			fprintf(file_to_print, designation);                                                     \
			if(operation_num_args == ONE_ARG) fprintf(file_to_print, "(");							 \
			PrintNode(expr_tree, node->right, node, file_to_print);									 \
			if(operation_num_args == ONE_ARG) fprintf(file_to_print, ")");							 \
			break;

		#include "opers_defs.h"

		default:
			assert(0);
	}
	
	#undef OPER_DEF

	if (IsBracketsNeeded(NODE_TYPE, parent_node_priority, node_priority))
		fprintf(file_to_print, ")");
}

void PlotTreeGraph(Tree* expr_tree, const char* graph_name)
{
	FILE* py_file = fopen("graph_plot.py", "w");

	fprintf(py_file, 
		"import numpy as np\n"
		"import matplotlib.pyplot as plt\n"
		"import sympy as sp\n"
		"x = sp.symbols('x')\n"
		"expr_str = \"");

	PrintNode(expr_tree, expr_tree->root, nullptr, py_file);

	fprintf(py_file, 
		"\"\n"
		"expr = sp.sympify(expr_str)\n"
		"my_function = sp.lambdify(x, expr, 'numpy')\n"
		"x_values = np.linspace(-10, 10, 100)\n"
		"y_values = my_function(x_values)\n"
		"plt.plot(x_values, y_values, label=expr_str)\n"
		"plt.title('Bailando %s: {}'.format(expr_str))\n"
		"plt.xlabel('x')\n"
		"plt.ylabel('y')\n"
		"plt.legend()\n"
		"plt.grid(True)\n"
		"plt.savefig('Bailando %s.png')\n"
		"plt.show()\n", 
		graph_name, graph_name);

	fclose(py_file);

	system("python graph_plot.py");

	DO_LATEX_PRINTF(
		"\\begin{figure}\n"
		"\\centering\n"
		"\\includegraphics[width=0.8\\linewidth]{Bailando %s.png}\n"
		"\\caption{Bailando: %s}\n"
		"\\label{fig:my_image}\n"
		"\\end{figure}\n"
		, graph_name, graph_name);
}

void PlotTaylorDiffGraph (Tree* expr_tree, const char* graph_name, char var, size_t serie_length, double point)
{
	Tree taylor_tree      = GetTaylorSerieTree(*expr_tree, var, serie_length, point);
	Tree taylor_diff_tree = {};
	TreeCtor(&taylor_diff_tree, DifferentiatorElemCtor, DifferentiatorElemDtor, DifferentiatorElemPrinter);
	taylor_diff_tree.root = CreateOperNode(&taylor_diff_tree, SUB, expr_tree->root, taylor_tree.root);
	
	OptimizeTree(&taylor_diff_tree);

	PlotTreeGraph(&taylor_diff_tree, graph_name);

	TreeDtor(&taylor_diff_tree);
	TreeDtor(&taylor_tree);
}

void PlotTaylorTreeGraph (Tree* expr_tree, const char* graph_name, char var, size_t serie_length, double point)
{
	Tree taylor_tree = GetTaylorSerieTree(*expr_tree, var, serie_length, point);
	OptimizeTree(&taylor_tree);

	PlotTreeGraph(&taylor_tree, graph_name);

	TreeDtor(&taylor_tree);
}

char* DifferentiatorElemPrinter(const TreeNode_t* elem_to_print)
{
	assert(elem_to_print != nullptr);

	static char str_to_output[MAX_COMMAND_LENGTH] = "";

	if (elem_to_print->type == NUM)
	{
		if (IsDoubleInt(elem_to_print->elem.num))
			sprintf(str_to_output, "%d", (int)elem_to_print->elem.num);
		else
			sprintf(str_to_output, "%lf", elem_to_print->elem.num);
	}
	else if (elem_to_print->type == VAR)
		sprintf(str_to_output, "%c", elem_to_print->elem.var.name);
	else if (elem_to_print->type == OPER)
		return GetOperDesignation(elem_to_print->elem.oper);
	else
		assert(0);
	
	return str_to_output;
}

unsigned IntermediatePrinterCtor(IntermediatePrinter* printer)
{
	assert(printer != nullptr);

	printer->current_str_num = 0;

	printer->latex_file = fopen("nedotex.tex", "w"); // TODO: обработать + printer for n diffs
	if (!printer->latex_file)
		return INTERMEDIATE_PRINTER_ERROR;

	DO_LATEX_PRINTF(
		"\\documentclass{article} \n"
		"\\usepackage{graphicx}\n"
		"\\title{Bailando Differentiator} \n"
		"\\author{Dmit} \n"
		"\\date{December 2023} \n"
		"\\begin{document} \n"
		"\\maketitle \n"
		"\\section{Bailando-Differentiating Func} \n");

	return TextInfoCtor(&printer->prints_text_info, "bailando.txt"); // TODO: подать файл по-другому мб
}

unsigned IntermediatePrinterDtor(IntermediatePrinter* printer)
{
	assert(printer != nullptr);

	DO_LATEX_PRINTF("\n\\end{document}");

	fclose(printer->latex_file);

	return TextInfoDtor(&printer->prints_text_info); // TODO: мб другую ошибку возвращаться
}