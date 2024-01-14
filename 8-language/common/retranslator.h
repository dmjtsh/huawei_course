#ifndef BACKEND_H
#define BACKEND_H

#include "../frontend/src/syntax_analizer.h"

#include "DimasLIB/DimasTree/tree.h"
#include "nametable.h"

void RetranslateNode(Tree* tree, TreeNode* current_node, NameTable* nametable, FILE* asm_code_file);

#endif BACKEND_H