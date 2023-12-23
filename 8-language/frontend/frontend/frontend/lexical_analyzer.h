#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "DimasLIB/DimasTree/tree.h"
#include "frontend.h"

TreeNode** DoLexicalAnalisys(Tree* expr_tree, const char* file_name, NameTable* name_table);

#endif