#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>

#include "TX/TXLib.h"
#include "tree.h"
#include "DimasLIB/DimasStack/stack.h"
#include "DimasLIB/DimasTextInfo/text_info.h"

#define SSCANF_WITH_CHECK(scanf_format, input_elem)				\
	do															\
	{															\
		size_t valid_inputs_num = 0;							\
		do														\
		{														\
			valid_inputs_num = scanf(scanf_format, input_elem);	\
		}													    \
		while(valid_inputs_num != 1);							\
	} while(0)

#define ARR_LEN(arr) sizeof(arr) / sizeof(arr[0])

const int SYNTAX_ERROR = -1;

// YES AND NO USER INPUTS
const char YES_INPUT = 'y';	
const char  NO_INPUT = 'n';
constexpr const char YES_NO_USER_INPUTS[2] = {YES_INPUT, NO_INPUT};

// GAME MODES
const char GUESS_GAME_MODE = 'g';
const char COMP_GAME_MODE  = 'c';
const char DEF_GAME_MODE   = 'd';
constexpr const char GAME_MODES[3] = {GUESS_GAME_MODE, COMP_GAME_MODE, DEF_GAME_MODE};

struct Akinator
{
	Stack node_properties;

	Tree tree;

	unsigned errors;
	FILE* logger;
	FILE* graph;
	TextInfo text_info;
};

unsigned AkinatorPerformGame(Akinator* akinator);

unsigned AkinatorCtor(Akinator* akinator);
unsigned AkinatorDtor(Akinator* akinator);

#endif // AKINATOR_H