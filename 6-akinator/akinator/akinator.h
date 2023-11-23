#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>

#include "DimasLIB/DimasTextInfo/text_info.h"

const size_t MAX_NODE_STR_LEN = 60;

struct Node
{
	char elem[MAX_NODE_STR_LEN];

	Node* left;
	Node* right;
};

Node* OpNew (char* data);

void OpDelete (Node* node);

enum AkinatorErrorBits
{
	AKINATOR_PTR_NULL        = 1 << 0,
	ROOT_PTR_NULL            = 1 << 1,
	NODE_CALLOC_ERROR        = 1 << 2,
	AKINATOR_BAD_SIZE        = 1 << 3,
	AKINATOR_LOGER_ERROR     = 1 << 4,
	AKINATOR_GRAPH_ERROR     = 1 << 5,
	TREE_STR_INVALID         = 1 << 6,
	AKINATOR_TEXT_INFO_ERROR = 1 << 7
};

const size_t AKINATOR_MAX_SIZE = 10e15;

struct Akinator
{
	Node* root;
	size_t size;

	unsigned errors;

	FILE* logger;
	FILE* graph;
	TextInfo text_info;
};

const char YES_INPUT = 'y';	
const char  NO_INPUT = 'n';

unsigned AkinatorPerformGame(Akinator* akinator);

unsigned AkinatorCtor (Akinator* akinator);

unsigned AkinatorDtor (Akinator* akinator);

const int SYNTAX_ERROR = -1;

void PrintNode(const Node* node, FILE* logger);

size_t ReadNode(Node** node_ptr, const char* tree_text_repr, unsigned* errors_ptr);

#endif // AKINATOR_H