#pragma once

#include "../../common/DimasLIB/DimasTree/tree.h"
#include "../../common/program_nametables.h"
#include "../../common/nametable.h"

Tree GetCodeTree(const char* file_name, ProgramNameTables* nametables);

void PrintCurrentToken(TreeNode** tokens, size_t* current_token_num);