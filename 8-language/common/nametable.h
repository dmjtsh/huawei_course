#pragma once

#include <stdio.h>

const size_t MAX_NAMETABLE_HEADER_SIZE = 50;
const size_t MAX_NAMETABLE_SIZE        = 100;
const size_t MAX_ID_SIZE               = 200;

enum NameTableElemType
{
	VARIABLE, FUNCTION, WRONG_NT_TYPE
};

struct NameTableElem
{
	NameTableElemType type;
	char              str[MAX_ID_SIZE];
	int               code;
};

struct NameTable
{
	NameTableElem elems[MAX_NAMETABLE_SIZE];
	size_t        size;
};

bool IsKeyword(NameTable* nametable, const char* name_str);

void           ReadNameTable (NameTable* nametable, FILE* ast_file, const char* nametable_header, unsigned* errors);
void           WriteNameTable(NameTable* nametable, const char* needed_nametable_header, FILE* output_file);
NameTableElem* NameTableAdd  (NameTable* nametable, const char* designation, int elem_code);
NameTableElem* NameTableFind (NameTable* nametable, const char* elem_to_find);