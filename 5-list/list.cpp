#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "list.h"
#include "error_processing.h"
#include "DimasLIB/DimasStack/stack.h"

void ListPrint(List* list, FILE* logger)
{
	Node current_node = list->data[FICT_ELEM_INDEX];
	for(size_t i = 0; i < list->size + 1; i++)
	{
		size_t current_node_index = list->data[current_node.next].prev;
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
		if (list->errors & LIST_BAD_FREE_STACK)        fprintf(logger, "SOME TROUBLES WITH FREE STACK\n");
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
	CHECK_ERROR(list, list->free.errors,               LIST_BAD_FREE_STACK)
	CHECK_ERROR(list, list->logger == NULL,            LIST_LOGER_ERROR)
	CHECK_ERROR(list, list->size     >= LIST_MAX_SIZE, LIST_BAD_SIZE)
	CHECK_ERROR(list, list->capacity >= LIST_MAX_SIZE, LIST_BAD_CAPACITY)
	CHECK_ERROR(list, list->capacity < list->size,     LIST_SIZE_GREATER_CAPACITY)

	return list->errors;
}

int ListDataRealloc(List* list, size_t new_capacity)
{
	assert(list !=  NULL);

	Node* new_data = (Node*)calloc(new_capacity, sizeof(Node));
	if (!new_data)
	{
		list->errors |= LIST_REALLOC_ERROR;
		return list->errors; 
	}
	
	int elems_counter = 0;


	// REALLOC DATA FROM DIFFERENT PARTS OF ARRAY
	if(list->size > 0)
	{
		list->head = 1;
		list->tail = list->size;

		Node current_node      = list->data[FICT_ELEM_INDEX];
		size_t next_elem_index = list->head;
		size_t prev_elem_index = list->tail;                // LOOPING LIST

		for(;elems_counter < list->size + 1; elems_counter++)
		{
			new_data[elems_counter]      = current_node;
			new_data[elems_counter].prev = prev_elem_index;
			new_data[elems_counter].next = next_elem_index;

			current_node     = list->data[current_node.next];
			prev_elem_index  = elems_counter;
			next_elem_index += 1;
		}

		new_data[elems_counter - 1].next = FICT_ELEM_INDEX; // LOOPING LIST
	}

	// FILL DATA WITH POISON AND FILL FREE STACK
	for(int i = new_capacity - 1; i > elems_counter - 1; i--)
	{
		LIST_ELEM_SET_POISON(new_data[i])
		StackPush(&list->free, i);
	}

	list->data     = new_data;
	list->capacity = new_capacity;

	return 0;
}

enum DataReallocAction
{
	INCREASE_DATA = 0,
	DECREASE_DATA = 1
};
int ListDataReallocIfNeeded(List* list, DataReallocAction realloc_action)
{
	assert(list != NULL);
	assert(list->data != NULL);

	const int INCREASE_COEFF = 4;
	const int DECREASE_COEFF = 2;

	switch (realloc_action)
	{
	case INCREASE_DATA:
		if (list->capacity <= list->size + 1)
			return ListDataRealloc(list, list->capacity * INCREASE_COEFF);
		break;
	case DECREASE_DATA:
		if (list->capacity > (list->size + 1) * DECREASE_COEFF)
			return ListDataRealloc(list, list->capacity / DECREASE_COEFF);
		break;
	}
	return 0;
}

unsigned ListInsertAfter(List* list, size_t elem_index, double new_elem_value)
{
	ERROR_PROCESSING(list, ListVerifier, ListDump, ListDtor)

	if(elem_index == ELEM_INDEX_POISON)
		return WRONG_COMMAND_USAGE;

	size_t free_elem_index = 0;
	StackPop(&list->free, (Elem_t*)&free_elem_index);

	size_t next_elem_index = list->data[elem_index].next;

	list->data[next_elem_index].prev  = free_elem_index;
	list->data[elem_index].next       = free_elem_index;

	list->data[free_elem_index].next  = next_elem_index;
	list->data[free_elem_index].prev  = elem_index;

	list->data[free_elem_index].value = new_elem_value;

	ListDataReallocIfNeeded(list, INCREASE_DATA);
	list->size++;

	list->head = list->data[FICT_ELEM_INDEX].next;
	list->tail = list->data[FICT_ELEM_INDEX].prev;

	return 0;
}

unsigned ListInsertBefore(List* list, size_t elem_index, double new_elem_value)
{
	ERROR_PROCESSING(list, ListVerifier, ListDump, ListDtor)

	if(elem_index == ELEM_INDEX_POISON)
		return WRONG_COMMAND_USAGE;

	ListDataReallocIfNeeded(list, INCREASE_DATA);

	size_t free_elem_index = 0;
	StackPop(&list->free, (Elem_t*)&free_elem_index);

	size_t prev_elem_index = list->data[elem_index].prev;
	
	list->data[prev_elem_index].next  = free_elem_index;
	list->data[elem_index].prev       = free_elem_index;

	list->data[free_elem_index].next  = elem_index;
	list->data[free_elem_index].prev  = prev_elem_index;

	list->data[free_elem_index].value = new_elem_value;
	
	list->size++;
	
	list->head = list->data[FICT_ELEM_INDEX].next;
	list->tail = list->data[FICT_ELEM_INDEX].prev;

	return 0;
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

	StackPush(&list->free, (Elem_t)elem_index);

	list->size--;
	
	ListDataReallocIfNeeded(list, DECREASE_DATA);
	
	list->head = list->data[FICT_ELEM_INDEX].next;
	list->tail = list->data[FICT_ELEM_INDEX].prev;

	return 0;
}

unsigned ListCtor(List* list)
{
	if(!list)
		return LIST_PTR_NULL;
	
	if(StackCtor(&list->free))
		list->errors |= LIST_BAD_FREE_STACK;

	ListDataRealloc(list, LIST_START_CAPACITY);
	
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

	unsigned destructor_errors = 0;
	if (StackDtor(&list->free))
		destructor_errors |= LIST_BAD_FREE_STACK;

	return destructor_errors;
}