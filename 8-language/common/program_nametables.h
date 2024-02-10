#pragma once

#include "nametable.h"

//
//
// TODO: DOCS: SCOPES == FUNCTION SCOPES 
//
//

#define FUNCS_COUNT				 nametables->functions_nametable.size
#define FUNCS_NAMETABLE			 nametables->functions_nametable
#define CURRENT_SCOPE_NUM	     nametables->scopes_counter
#define CURRENT_SCOPES_NAMETABLE nametables->scopes_nametables[FUNCS_COUNT - 1]  // TODO: фиксануть мб как-то

struct ProgramNameTables
{
	NameTable  functions_nametable;
	NameTable* scopes_nametables;
	NameTable  common_nametable;

	size_t scopes_counter;
	size_t vars_counter;
};

void WriteProgramNameTables(ProgramNameTables* nametables, FILE* input_file);
void ReadProgramNameTables (ProgramNameTables* nametables, FILE* ast_file, unsigned* errors);

NameTableElem* ProgramNameTablesAddVar (ProgramNameTables* nametables, char* var_name);
NameTableElem* ProgramNameTablesAddFunc(ProgramNameTables* nametables, char* func_name);

void ProgramNameTablesCtor(ProgramNameTables* nametables, size_t scopes_counter);
void ProgramNameTablesDtor(ProgramNameTables* nametables);
