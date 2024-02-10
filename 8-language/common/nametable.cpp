#define _CRT_SECURE_NO_WARNINGS

#include "nametable.h"
#include "ast_tree.h"
#include "TX/TXLib.h"
#include "DimasLIB/DimasTree/tree.h"

#include <assert.h>
#include <string.h>

bool IsKeyword(NameTable* nametable, const char* name_str)
{
	for(size_t i = 0; i < OPER_COUNT; i++)
	{
		if (strcmp(nametable->elems[i].str, name_str) == 0)
			return true;
	}

	return false;
}

#define CHECK_VALID_ARGS_NUM(_fscanf)      \
	{                                      \
		valid_args_num = _fscanf;          \
		if(!(valid_args_num))              \
		{                                  \
			*errors |= INVALID_AST_FORMAT; \
			return;                        \
		}                                  \
	}

void ReadNameTable(NameTable* nametable, FILE* ast_file, const char* needed_nametable_header, unsigned* errors)
{
	assert(nametable != nullptr);
	assert(ast_file  != nullptr);

	size_t valid_args_num = 0;
	int cmd_len           = 0;
	
	char nametable_header[MAX_NAMETABLE_HEADER_SIZE] = "";
	CHECK_VALID_ARGS_NUM(fscanf(ast_file, " %s [ %zu ] {", nametable_header, &nametable->size));

	if(strcmp(needed_nametable_header, nametable_header) != 0)
		*errors |= INVALID_AST_FORMAT;

	for(size_t i = 0; i < nametable->size; i++)
	{
		CHECK_VALID_ARGS_NUM(fscanf(ast_file, " [ \" %[^\"] \" , %d]", 
		nametable->elems[i].str, &nametable->elems[i].code));
	}

	(void)fscanf(ast_file, " }%n", &cmd_len);

	if(!cmd_len)
		*errors |= INVALID_AST_FORMAT;
}

#undef CHECK_VALID_ARGS_NUM

void WriteNameTable(NameTable* nametable, const char* nametable_header, FILE* output_file)
{
	assert(nametable != nullptr);

	fprintf(output_file, "%s [%zu] {\n", nametable_header, nametable->size);

	for (size_t i = 0; i < nametable->size; i++) 
	{
		fprintf(output_file, "    [\"%s\", %d]\n", 
			nametable->elems[i].str, nametable->elems[i].code);
	}

	fprintf(output_file, "}\n\n");
}

NameTableElem* NameTableAdd(NameTable* nametable, const char* designation, int elem_code)
{
	assert(nametable  != nullptr);
	assert(designation != nullptr);

	size_t curr_elem_index = nametable->size;
	
	strcpy(nametable->elems[curr_elem_index].str, designation);
	nametable->elems[curr_elem_index].code = elem_code;
	
	nametable->size++;

	if(nametable->size == MAX_NAMETABLE_SIZE)
		return nullptr;

	return &nametable->elems[curr_elem_index];
}

NameTableElem* NameTableFind(NameTable* nametable, const char* elem_to_find)
{
	for(size_t i = 0; i < nametable->size; i++)
	{
		if(strcmp(nametable->elems[i].str, elem_to_find) == 0)
			return &nametable->elems[i];
	}

	return nullptr;
}

NameTableElem* NameTableFind(NameTable* nametable, const char* elem_to_find, size_t* elem_num)
{
	for(size_t i = 0; i < nametable->size; i++)
	{
		if(strcmp(nametable->elems[i].str, elem_to_find) == 0)
		{
			*elem_num = i;
			return &nametable->elems[i];
		}
	}

	return nullptr;
}

void NameTableCtor(NameTable* nametable)
{
	assert(nametable != nullptr);
}

void NameTableDtor(NameTable* nametable)
{
	assert(nametable != nullptr);
}