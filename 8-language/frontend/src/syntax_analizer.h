#pragma once

#include "../../common/DimasLIB/DimasTree/tree.h"
#include "../../common/nametable.h"

Tree GetCodeTree(const char* file_name, NameTable* nametable);

void PrintCurrentToken(TreeNode** tokens, size_t* current_token_num);