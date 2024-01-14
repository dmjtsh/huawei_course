#pragma once

#include <stdio.h>

#include "DimasLIB/DimasTree/tree.h"

const size_t MAX_NAMES_TABLE_SIZE = 100;

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

void           NameTableRead(NameTable* nametable, FILE* ast_file, unsigned* errors);
void		   NameTableWrite(NameTable* nametable, FILE* output_file);
void           NameTableAdd  (NameTable* nametable, const char* designation, KeyWordType elem_type, int elem_code);
NameTableElem* NameTableFind (NameTable* nametable, const char* elem_to_find);
void           NameTableCtor (NameTable* nametable);
void           NameTableDtor (NameTable* nametable);