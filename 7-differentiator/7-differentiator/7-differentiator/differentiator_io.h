#ifndef DIFFERENTIATOR_IO
#define DIFFERENTIATOR_IO

#define DO_LATEX_PRINTF(...) fprintf(INTERMEDIATE_PRINTER.latex_file, __VA_ARGS__)

void PlotTreeGraph       (Tree* expr_tree, const char* graph_name);
void PlotTaylorTreeGraph (Tree* expr_tree, const char* graph_name, char var, size_t serie_length, double point);
void PlotTaylorDiffGraph (Tree* expr_tree, const char* graph_name, char var, size_t serie_length, double point);

void PrintNode     (const Tree* expr_tree, const TreeNode* node, const TreeNode* parent_node, FILE* file_to_print);
void PrintLatexNode(const Tree* expr_tree, const TreeNode* node, const TreeNode* parent_node);
void DoIntermidiatePrint(Tree* expr_tree, const TreeNode* node);

const unsigned INTERMEDIATE_PRINTER_ERROR = 1 << 1;
unsigned IntermediatePrinterCtor(IntermediatePrinter* printer);
unsigned IntermediatePrinterDtor(IntermediatePrinter* printer);

char* DifferentiatorElemPrinter(const TreeNode_t* elem_to_print);

#endif