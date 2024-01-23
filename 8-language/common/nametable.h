#pragma once

#include <stdio.h>

const size_t MAX_NAMES_TABLE_SIZE = 100;
const size_t MAX_ID_SIZE          = 200;

enum KeyWordType
{
	KEYWORD, VARIABLE, FUNCTION
};

struct NameTableElem
{
	KeyWordType type;
	char        str[MAX_ID_SIZE];
	int         code;
};

struct NameTable
{
	NameTableElem elems[MAX_NAMES_TABLE_SIZE];
	size_t size;
};

bool IsKeyword(NameTable* nametable, const char* name_str);

void           ReadNameTable(NameTable* nametable, FILE* ast_file, unsigned* errors);
void		   WriteNameTable(NameTable* nametable, FILE* output_file);
NameTableElem* NameTableAdd(NameTable* nametable, const char* designation, KeyWordType elem_type, int elem_code);
NameTableElem* NameTableFind (NameTable* nametable, const char* elem_to_find);
void           NameTableCtor (NameTable* nametable);
void           NameTableDtor (NameTable* nametable);