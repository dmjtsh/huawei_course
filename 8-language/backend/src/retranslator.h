#pragma once

#include <stdio.h>

#include "../../common/nametable.h"
#include "../../common/DimasLIB/DimasTree/tree.h"

void RetranslateTree(Tree* tree, ProgramNameTables* nametables, FILE* asm_code_file);

void ReadTreeAndNameTables(ProgramNameTables* nametables, Tree* tree, const char* ast_file_name);
