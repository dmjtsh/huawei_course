#include <assert.h>

#include "list_io.h"

void ListPrint(List* list, FILE* logger)
{
	assert(list != NULL);
	assert(logger != NULL);

	fprintf(logger, "nodes:\n");

	Node current_node = list->data[FICT_ELEM_INDEX];
	size_t current_node_index = 0;
	for(size_t i = 0; i < list->size + 1; i++)
	{
		current_node_index = list->data[current_node.next].prev;
		fprintf(logger,   " | \n");

		fprintf(logger, "(%zu)[%lf] prev: %zu next: %zu", 
		current_node_index, current_node.value, current_node.prev, current_node.next);
		if (current_node_index == list->head)
			fprintf(logger, " <- head");
		if (current_node_index == list->tail)
			fprintf(logger, " <- tail");

		fprintf(logger, "\n");

		current_node = list->data[current_node.next];
	}

	fprintf(logger, "\nfree nodes:\n");

	current_node_index = list->free;
	for (int i = 0; i < list->capacity + 1 - list->size - 1; i++)
	{
		fprintf(logger, " | \n");
		fprintf(logger, "[%zu] prev: %zu\n", current_node_index, list->data[current_node_index].prev);
		current_node_index = list->data[current_node_index].prev;
	}
}

void ListGraphDump(List* list)
{
	assert(list != NULL);

	fprintf(list->graph, "digraph ListGraph {\n"
						 "rankdir=\"LR\";\n");
	// CREATING ELEMS
	for(int i = 0; i < list->capacity + 1; i++)
	{
		if (list->data[i].next == ELEM_INDEX_POISON)
		{
			fprintf(list->graph, 
			"node [shape=\"box\", style=\"filled\", fillcolor=\"#93ffc5\", fontcolor=\"#0e0a2a\", margin=\"0.01\"];\n");
		}
		else if (i == FICT_ELEM_INDEX)
		{
			fprintf(list->graph, 
			"node [shape=\"box\", style=\"filled\", fillcolor=\"#24b0ff\", fontcolor=\"#0e0a2a\", margin=\"0.01\"];\n");
		}
		fprintf(list->graph, 
		"\"Node%d\" [shape=\"record\", label=\"\\n Node: %d \\n\\n|\\n Value:" VALUE_T_SPECIFIER "\\n\\n | Prev: %d | Next: %d\"];\n",
		i, i, list->data[i].value, list->data[i].prev, list->data[i].next);	

		fprintf(list->graph, 
		"node [shape=\"box\", style=\"filled\", fillcolor=\"#c3c1f1\", fontcolor=\"#0e0a2a\", margin=\"0.01\"];\n");
	}

	// INVISIBLE LINES FOR ALIGNMENT
	for(int i = 0; i < list->capacity; i++) 
	{
		fprintf(list->graph, "\"Node%d\" -> \"Node%d\" [color=\"#43ff6400\"];", i, i+1);
	}
	
	// CREATING LINKS BETWEEN ELEMS
	for(int i = 0; i < list->capacity + 1; i++)
	{
		if (list->data[i].prev != ELEM_INDEX_POISON)
		{
			fprintf(list->graph, "\"Node%d\" -> \"Node%d\"  [color=\"#ff5b23\" fontcolor=\"#ff5b23\"];\n", 
			i, list->data[i].prev);
		}

		if (list->data[i].next != ELEM_INDEX_POISON)
		{
			fprintf(list->graph, "\"Node%d\" -> \"Node%d\" [color=\"#7540ff\" fontcolor=\"#7540ff\"];\n", 
			i, list->data[i].next);
		}
	}
	
	// Creating LIST OBJ, FREE PTR, HEAD PTR, TAIL PTR
	fprintf(list->graph, 
	"node [shape=\"box\", style=\"filled\", fillcolor=\"#ffdf83\", fontcolor=\"#0e0a2a\", margin=\"0.1\"];\n"
	"List [shape=record, label=\"\\n DIMAS LIST \\n \\n | \\n Capacity: 7 \\n \\n | \\n Size: 4 \\n \\n\" , fontsize=18];\n"

	"node [shape=box, style=filled, fillcolor=\"#93ffc5\", fontcolor=\"#0e0a2a\", margin=\"0.01\"];\n"
	"Free [shape=record, label=\"Free\", fontsize=16];\n"

	"node [shape=box, style=filled, fillcolor=\"#ff0eef\", fontcolor=\"#0e0a2a\", margin=\"0.01\"];\n"
	"Head [shape=record, label=\"Head\", fontsize=16];\n"

	"node [shape=box, style=filled, fillcolor=\"#ff0eef\", fontcolor=\"#0e0a2a\", margin=\"0.01\"];\n"
	"Tail [shape=record, label=\"Tail\", fontsize=16];\n");

	fprintf(list->graph, "\"List\" -> \"Node0\" [weight=1, color=\"#43ff6400\", headport=n, tailport=n];\n"); // INVISIBLE LINK FOR ALIGNMENT

	// CREATING LINKS BETWEEN FREE PTR, HEAD PTR, TAIL PTR
	fprintf(list->graph, 
	"\"Free\" -> \"Node%d\" [headport=n, tailport=s, constraint=true, fillcolor=\"#93ffc5\"];\n"
	"\"Tail\" -> \"Node%d\" [headport=n, tailport=s, constraint=true, fillcolor=\"#ff0eef\"];\n"
	"\"Head\" -> \"Node%d\" [headport=n, tailport=s, constraint=true, fillcolor=\"#ff0eef\"];\n",
	list->free, list->tail, list->head);
	
    fprintf(list->graph,"}");
}

void ListDump(List* list, FILE* logger)
{
	assert(list   != NULL);
	assert(logger != NULL);

	static size_t num_of_call = 1;
	fprintf(logger, 
	"=======================================\n"
	"List DUMP CALL #%zu\n", num_of_call);
	if (list->errors)
	{
		fprintf(logger,
		"-------------ERRORS------------\n");
		if (list->errors & LIST_PTR_NULL)
		{
			fprintf(logger, "List POINTER IS NULL\n");
			return;
		}
		if (list->errors & LIST_LOGER_ERROR)           fprintf(logger, "List LOGER ERROR\n");
		if (list->errors & LIST_DATA_PTR_NULL)         fprintf(logger, "List Data Ptr Null\n");
        if (list->errors & LIST_SIZE_GREATER_CAPACITY) fprintf(logger, "List Size GREATER Capacity\n");
        if (list->errors & LIST_SIZE_LESS_ONE)         fprintf(logger, "List Size LESS One\n");
        if (list->errors & LIST_REALLOC_ERROR)         fprintf(logger, "List REALLOC ERROR\n");
        if (list->errors & LIST_BAD_SIZE)              fprintf(logger, "List Size TOO BIG\n");
        if (list->errors & LIST_BAD_CAPACITY)          fprintf(logger, "List Capacity TOO BIG\n");
		if (list->errors & LIST_GRAPH_ERROR)           fprintf(logger, "List Graph Error\n");

		fprintf(logger,
		"----------END_OF_ERRORS--------\n");
	}
	else
	{
		fprintf(logger,
		"------------NO_ERRORS----------\n");

		ListPrint(list, logger);
	}
	
	fprintf(logger,
	"=======================================\n\n");

	num_of_call++;
}							  
