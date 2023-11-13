#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "list.h"
#include "error_processing.h"

void ListPrint(List* list, FILE* logger)
{
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
	for (int i = 0; i < list->capacity - list->size - 1; i++)
	{
		fprintf(logger, " | \n");
		fprintf(logger, "[%zu] prev: %zu\n", current_node_index, list->data[current_node_index].prev);
		current_node_index = list->data[current_node_index].prev;
	}
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

unsigned ListVerifier(List* list)
{
	if (!list)
		return LIST_PTR_NULL;

	CHECK_ERROR(list, list->data == NULL,              LIST_DATA_PTR_NULL)
	CHECK_ERROR(list, list->logger == NULL,            LIST_LOGER_ERROR)
	CHECK_ERROR(list, list->size     >= LIST_MAX_SIZE, LIST_BAD_SIZE)
	CHECK_ERROR(list, list->capacity >= LIST_MAX_SIZE, LIST_BAD_CAPACITY)
	CHECK_ERROR(list, list->capacity < list->size,     LIST_SIZE_GREATER_CAPACITY)

	return list->errors;
}

unsigned ListDataReallocUp(List* list, size_t new_capacity)
{
	assert(list !=  NULL);

	Node* new_data = (Node*)realloc(list->data, new_capacity * sizeof(Node));
	if (!new_data)
	{
		list->errors |= LIST_REALLOC_ERROR;
		return list->errors; 
	}

	list->data     = new_data;
	list->capacity = new_capacity;

	// FILL DATA WITH POISON AND CONNECT FREE
	for(int i = list->size + 1; i < list->capacity; i++)
	{
		list->data[i].prev = list->free;
		list->free = i;

		list->data[list->free].next  = ELEM_INDEX_POISON; // POISON
		list->data[list->free].value = ELEM_VALUE_POISON; // POISON
	}

	return 0;
}

size_t ListInsertAfter(List* list, size_t elem_index, double new_elem_value)
{
	ERROR_PROCESSING(list, ListVerifier, ListDump, ListDtor)

	if(elem_index == ELEM_INDEX_POISON)
		return WRONG_COMMAND_USAGE;

	size_t next_elem_index      = list->data[elem_index].next;
	size_t free_prev_elem_index = list->data[list->free].prev;
	size_t free_elem_index      = list->free;

	list->data[next_elem_index].prev  = free_elem_index;
	list->data[elem_index].next       = free_elem_index;

	list->data[free_elem_index].next  = next_elem_index;
	list->data[free_elem_index].prev  = elem_index;
	list->data[free_elem_index].value = new_elem_value;

	list->free = free_prev_elem_index;

	list->size++;
	
	if(list->size + 1 >= list->capacity)
		ListDataReallocUp(list, list->capacity * REALLOC_UP_COEFF);

	list->head = list->data[FICT_ELEM_INDEX].next;
	list->tail = list->data[FICT_ELEM_INDEX].prev;

	return free_elem_index;
}

size_t ListInsertBefore(List* list, size_t elem_index, double new_elem_value)
{
	ERROR_PROCESSING(list, ListVerifier, ListDump, ListDtor)

	if(elem_index == ELEM_INDEX_POISON)
		return WRONG_COMMAND_USAGE;

	size_t prev_elem_index      = list->data[elem_index].prev;
	size_t free_prev_elem_index = list->data[list->free].prev;
	size_t free_elem_index      = list->free;

	list->data[prev_elem_index].next = free_elem_index;
	list->data[elem_index].prev      = free_elem_index;

	list->data[free_elem_index].next  = elem_index;
	list->data[free_elem_index].prev  = prev_elem_index;
	list->data[free_elem_index].value = new_elem_value;
	
	list->free = free_prev_elem_index;

	list->size++;

	if(list->size + 1 >= list->capacity)
		ListDataReallocUp(list, list->capacity * REALLOC_UP_COEFF);
	
	list->head = list->data[FICT_ELEM_INDEX].next;
	list->tail = list->data[FICT_ELEM_INDEX].prev;

	return free_elem_index;
}

unsigned ListRemove(List* list, size_t elem_index)
{
	ERROR_PROCESSING(list, ListVerifier, ListDump, ListDtor)

	if(elem_index == FICT_ELEM_INDEX || elem_index == ELEM_INDEX_POISON)
		return WRONG_COMMAND_USAGE;
	
	size_t next_elem_index = list->data[elem_index].next;
	size_t prev_elem_index = list->data[elem_index].prev;	
	
	list->data[prev_elem_index].next = list->data[elem_index].next;
	list->data[next_elem_index].prev = list->data[elem_index].prev;

	list->data[elem_index].prev = list->free;
	list->free = elem_index;

	list->data[list->free].next  = ELEM_INDEX_POISON; // POISON
	list->data[list->free].value = ELEM_VALUE_POISON; // POISON

	list->size--;
	
	list->head = list->data[FICT_ELEM_INDEX].next;
	list->tail = list->data[FICT_ELEM_INDEX].prev;

	return 0;
}

unsigned ListCtor(List* list)
{
	if(!list)
		return LIST_PTR_NULL;

	list->free = LIST_FREE_START_INDEX;

	ListDataReallocUp(list, LIST_START_CAPACITY);
	
	fopen_s(&list->logger, "list_logger.txt", "w");

	list->data[FICT_ELEM_INDEX].next  = FICT_ELEM_INDEX;
	list->data[FICT_ELEM_INDEX].prev  = FICT_ELEM_INDEX;
	list->data[FICT_ELEM_INDEX].value = FICT_ELEM_VALUE;

	list->head = FICT_ELEM_INDEX;
	list->tail = FICT_ELEM_INDEX;

	ERROR_PROCESSING(list, ListVerifier, ListDump, ListDtor)
	
	return 0;
}

unsigned ListDtor(List* list)
{
	if(!list)
		return LIST_PTR_NULL;

	free(list->data);

	fclose(list->logger);

	return 0;
}