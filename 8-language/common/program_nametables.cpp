#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "program_nametables.h"
#include "nametable.h"

void ReadProgramNameTables(ProgramNameTables* nametables, FILE* ast_file, unsigned* errors)
{
	assert(nametables != nullptr);
	assert(ast_file   != nullptr);
	assert(errors     != nullptr);

	assert(fscanf(ast_file, "Scopes Count: %zu", &nametables->scopes_counter) == 1);
	ProgramNameTablesCtor(nametables, nametables->scopes_counter);

	ReadNameTable(&nametables->functions_nametable, ast_file, "FunctionsNameTable", errors);

	for(size_t i = 0; i < nametables->scopes_counter; i++)
		ReadNameTable(&nametables->scopes_nametables[i], ast_file, "ScopeNameTable", errors);
}

void WriteProgramNameTables(ProgramNameTables* nametables, FILE* input_file)
{
	assert(nametables != nullptr);
	assert(input_file != nullptr);

	fprintf(input_file, "Scopes Count:%zu\n\n", nametables->scopes_counter);

	WriteNameTable(&nametables->functions_nametable, "FunctionsNameTable", input_file);

	for(size_t i = 0; i < nametables->scopes_counter; i++)
		WriteNameTable(&nametables->scopes_nametables[i], "ScopeNameTable", input_file);
}

NameTableElem* ProgramNameTablesAddVar(ProgramNameTables* nametables, char* var_name)
{
	assert(nametables != nullptr);
	assert(var_name   != nullptr);

	NameTableElem* var_id = nullptr;

	if(var_id = NameTableFind(&CURRENT_SCOPES_NAMETABLE, var_name))
		return var_id;
	
	var_id = NameTableAdd(&CURRENT_SCOPES_NAMETABLE, var_name, nametables->vars_counter); 
	nametables->vars_counter++;

	var_id->type = VARIABLE;

	return var_id;
}

NameTableElem* ProgramNameTablesAddFunc(ProgramNameTables* nametables, char* func_name)
{
	assert(nametables != nullptr);
	assert(func_name  != nullptr);

	if(!NameTableFind(&FUNCS_NAMETABLE, func_name))
	{
		NameTableElem* func_id = NameTableAdd(&nametables->functions_nametable, func_name, FUNCS_COUNT);

		func_id->type = FUNCTION;

		return func_id;
	}
	
	return nullptr;
}

void ProgramNameTablesCtor(ProgramNameTables* nametables, size_t scopes_counter)
{
	assert(nametables != nullptr);

	nametables->scopes_nametables = (NameTable*)calloc(scopes_counter, sizeof(NameTable));
	assert(nametables->scopes_nametables != nullptr);

	nametables->scopes_counter = scopes_counter;

	nametables->vars_counter = 0;
	nametables->functions_nametable.size = 0;
	
	for(size_t i = 0; i < scopes_counter; i++)
		nametables->scopes_nametables[i].size = 0;
}

void ProgramNameTablesDtor(ProgramNameTables* nametables)
{
	assert(nametables != nullptr);

	free(nametables->scopes_nametables);
}
