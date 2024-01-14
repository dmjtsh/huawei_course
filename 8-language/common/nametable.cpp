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

void NameTableRead(NameTable* nametable, FILE* ast_file, unsigned* errors)
{
	assert(nametable != nullptr);
	assert(ast_file  != nullptr);

	size_t valid_args_num = 0;

	CHECK_VALID_ARGS_NUM(fscanf(ast_file, " NameTable [ %zu ] {", &nametable->size));

	for(size_t i = 0; i < nametable->size; i++)
	{
		CHECK_VALID_ARGS_NUM(fscanf(ast_file, " [ \" %[^\"] \" , %d , %d ]", 
			nametable->elems[i].str, &nametable->elems[i].code, &nametable->elems[i].type));
	}

	CHECK_VALID_ARGS_NUM(fscanf(ast_file, " }"));
}

#undef CHECK_VALID_ARGS_NUM

void NameTableWrite(NameTable* nametable, FILE* output_file)
{
	assert(nametable != nullptr);

	fprintf(output_file, "NameTable [%zu] {\n", nametable->size);

	for (size_t i = 0; i < nametable->size; i++) 
	{
		fprintf(output_file, "    [\"%s\", %d, %d]\n", 
			nametable->elems[i].str, nametable->elems[i].code, nametable->elems[i].type);
	}

	fprintf(output_file, "}\n\n");
}

void NameTableAdd(NameTable* nametable, const char* designation, KeyWordType elem_type, int elem_code)
{
	assert(nametable  != nullptr);
	assert(designation != nullptr);

	int curr_elem_index = nametable->size;
	
	strcpy(nametable->elems[curr_elem_index].str, designation);
	nametable->elems[curr_elem_index].type = elem_type;
	nametable->elems[curr_elem_index].code = elem_code;
	
	nametable->size++;
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

void NameTableCtor(NameTable* nametable)
{
	assert(nametable != nullptr);

	#define OPER_DEF(code, designation, ...) NameTableAdd(nametable, designation, KEYWORD, code);

	#include "opers_defs.h"

	#undef OPER_DEF
}

void NameTableDtor(NameTable* nametable)
{
	;
}