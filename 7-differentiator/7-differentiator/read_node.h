#ifndef READ_NODE_H
#define READ_NODE_H

#include "DimasLIB/DimasTree/tree.h"

unsigned InitializeExprTree(Tree* expr_tree, const char* source_file);

const unsigned SOURCE_INVALID_SYNTAX = 1 << 1;

#endif