#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>

#include "../../common/nametable.h"
#include "../../common/DimasLIB/DimasTree/tree.h"

void RetranslateTree(Tree* tree, NameTable* nametable, FILE* asm_code_file);

#endif BACKEND_H