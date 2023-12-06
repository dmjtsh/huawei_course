#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdlib.h>

#include "list_io.h"

void ListDump(List* list, FILE* logger)
{
	assert(list   != NULL);
	assert(logger != NULL);

	static size_t num_of_call = 1;

	fprintf(logger, 
		"=======================================\n"
		"List DUMP CALL #%zu\n",
		num_of_call);

	if (list->errors)
	{
		fprintf(logger,
		"-------------ERRORS------------\n");
		if (list->errors & LIST_PTR_NULL)
		{
			fprintf(logger, "List POINTER IS NULL\n");
			return;
		}
		if (list->errors & LIST_LOGGER_ERROR)          fprintf(logger, "List LOGGER ERROR\n");
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

	}
	
	fprintf(logger,
	"=======================================\n\n");

	num_of_call++;
}							  


//void ListPrint(List* list, const char* reason_blya)
//{
//	assert(list != NULL);
//
//	fprintf(logger, "nodes:\n");
//
//	Node* current_node = &list->data[FICT_ELEM_INDEX];
//	size_t current_node_index = 0;
//	if(list->size == 0)
//		fprintf(logger, "(%zu)[%lf] prev: %zu next: %zu", 
//			current_node_index, current_node->value, current_node->prev, current_node->next);
//	else
//	{
//		for(size_t i = 0; i < list->size + 1; i++)
//		{
//			current_node_index = list->data[current_node->next].prev;
//
//			fprintf(logger, " | \n");
//			fprintf(logger, "(%zu)[%lf] prev: %zu next: %zu", 
//				current_node_index, current_node->value, current_node->prev, current_node->next);
//
//			if (current_node_index == list->head)
//				fprintf(logger, " <- head");
//			if (current_node_index == list->tail)
//				fprintf(logger, " <- tail");
//
//			fprintf(logger, "\n");
//
//			current_node = list->data + current_node->next;
//		}
//	}
//
//	fprintf(logger, "\nfree nodes:\n");
//
//	current_node_index = list->free;
//	for (int i = 0; i < list->capacity - list->size; i++)
//	{
//		fprintf(logger, " | \n");
//		fprintf(logger, "[%zu] next: %zu\n", current_node_index, list->data[current_node_index].next);
//		current_node_index = list->data[current_node_index].next;
//	}
//}

const char* HEAD_COLOR      = "#ff0eef";
const char* TAIL_COLOR      = "#ff0eef";
const char* FREE_COLOR      = "#93ffc5";
const char* LIST_INFO_COLOR = "#ffdf83";
const char* NEXT_COLOR      = "#7540ff";
const char* PREV_COLOR      = "#ff5b23";
const char* FICT_COLOR      = "#24b0ff";
const char* USUAL_COLOR     = "#c3c1f1";
const char* FONT_COLOR      = "#0e0a2a";
const char* INVISIBLE_COLOR = "#43ff6400";

const char* ListGraphPrint(List* list, const char* reason_blya)
{
	assert(list != NULL);

	fprintf(list->graph, "digraph ListGraph {\n"
						 "rankdir=\"LR\";\n");

	// CREATING ELEMS
	for(int i = 0; i < list->capacity + 1; i++)
	{
		if (list->data[i].prev == ELEM_INDEX_POISON)
			fprintf(list->graph,
				"node [shape=\"box\", style=\"filled\", fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.01\"];\n",
				 FREE_COLOR, FONT_COLOR);
		else if (i == FICT_ELEM_INDEX)
			fprintf(list->graph,
				"node [shape=\"box\", style=\"filled\", fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.01\"];\n",
				 FICT_COLOR, FONT_COLOR);
		else 
			fprintf(list->graph,
				"node [shape=\"box\", style=\"filled\", fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.01\"];\n",
				 USUAL_COLOR, FONT_COLOR);

		fprintf(list->graph,
			"\"Node%d\" [shape=\"record\", label=\"\\n Node: %d \\n\\n|\\n Value:" VALUE_T_SPECIFIER "\\n\\n | Prev: %d | Next: %d\"];\n",
			i, i, list->data[i].value, list->data[i].prev, list->data[i].next);
	}

	// INVISIBLE LINES FOR ALIGNMENT
	for(int i = 0; i < list->capacity; i++)
	{
		fprintf(list->graph, "\"Node%d\" -> \"Node%d\" [color=\"%s\"];", i, i+1, INVISIBLE_COLOR);
	}

	// CREATING LINKS BETWEEN ELEMS
	for(int i = 0; i < list->capacity + 1; i++)
	{
		if (list->data[i].prev != ELEM_INDEX_POISON)
		{
			fprintf(list->graph, "\"Node%d\" -> \"Node%d\"  [color=\"%s\" fontcolor=\"%s\"];\n",
				i, list->data[i].prev, PREV_COLOR, PREV_COLOR);
		}

		if (list->data[i].next != ELEM_INDEX_POISON)
		{
			fprintf(list->graph, "\"Node%d\" -> \"Node%d\" [color=\"%s\" fontcolor=\"%s\"];\n",
				i, list->data[i].next, NEXT_COLOR, NEXT_COLOR);
		}
	}

	// Creating LIST OBJ, FREE PTR, HEAD PTR, TAIL PTR
	fprintf(list->graph,
		"node [shape=\"box\", style=\"filled\", fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.1\"];\n"
		"List [shape=record, label=\"\\n DIMAS LIST \\n \\n | \\n Capacity: 7 \\n \\n | \\n Size: 4 \\n \\n\" , fontsize=18];\n"

		"node [shape=box, style=filled, fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.01\"];\n"
		"Free [shape=record, label=\"Free\", fontsize=16];\n"

		"node [shape=box, style=filled, fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.01\"];\n"
		"Head [shape=record, label=\"Head\", fontsize=16];\n"

		"node [shape=box, style=filled, fillcolor=\"%s\", fontcolor=\"%s\", margin=\"0.01\"];\n"
		"Tail [shape=record, label=\"Tail\", fontsize=16];\n",

		LIST_INFO_COLOR, FONT_COLOR,
		FREE_COLOR, FONT_COLOR,
		HEAD_COLOR, FONT_COLOR,
		TAIL_COLOR, FONT_COLOR);

	// INVISIBLE LINE FOR LIST_INFO ALLIGNMENT
	fprintf(list->graph, "\"List\" -> \"Node0\" [weight=1, color=\"%s\", headport=n, tailport=n];\n", INVISIBLE_COLOR);

	// CREATING LINKS BETWEEN FREE PTR, HEAD PTR, TAIL PTR
	fprintf(list->graph,
		"\"Free\" -> \"Node%d\" [headport=n, tailport=s, constraint=true, fillcolor=\"%s\"];\n"

		"\"Tail\" -> \"Node%d\" [headport=n, tailport=s, constraint=true, fillcolor=\"%s\"];\n"
		
		"\"Head\" -> \"Node%d\" [headport=n, tailport=s, constraint=true, fillcolor=\"%s\"];\n",
		
		list->free, FREE_COLOR,
		list->tail, TAIL_COLOR,
		list->head, HEAD_COLOR);

	fprintf(list->graph,"}\n");
    fflush (list->graph);

	static int num = 0;
	static char cmd[1000] = "";

	sprintf (cmd, "dot -T png list_graph.gv -o list_graph_%02d.png", num);

	system (cmd);

	sprintf (cmd, "list_graph_%02d.png", num);

	fprintf (list->logger, "<h2>PIDOR called this: %s:</h2>\n", reason_blya);
	fprintf (list->logger, "<img src=\"%s\" width = 50%%>\n", cmd);
	fprintf(list->logger, "<h1> ded pidor (no) </h1>");

	num++;
	return cmd;
}


// Primary Identifier Dedicated to Online Redirection


