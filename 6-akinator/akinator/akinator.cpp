#define _CRT_SECURE_NO_WARNINGS
#define TX_USE_SPEAK

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "TX/TXLib.h"
#include "akinator.h"
#include "akinator_errors.h"
#include "DimasLIB/DimasUtilities/error_processing.h"

#define SET_ERROR_AND_RET_IF_NEEDED(cond, error) \
do                                               \
{												 \
	if(!cond)									 \
	{											 \
		*errors_ptr |= error;					 \
		return 0;								 \
	}											 \
} while(0)

#define DO_AND_CHECK_SSCANF(check_cond, scanf_format)                                                           \
	do																										    \
	{																										    \
		read_args_num = sscanf(tree_text_repr + read_ch_count, scanf_format "%n", node_data, &current_str_len); \
		read_ch_count += current_str_len + 1;                                                                   \
		SET_ERROR_AND_RET_IF_NEEDED(check_cond, TREE_STR_INVALID);                                              \
	} while(0)

size_t ReadNode(Tree* tree, TreeNode** node_ptr, const char* tree_text_repr, unsigned* errors_ptr)
{
	assert(node_ptr       != nullptr);
	assert(tree_text_repr != nullptr);
	assert(errors_ptr     != nullptr);         

	if(*errors_ptr) return 0;       

	size_t read_ch_count   = 0;
	size_t current_str_len = 0;
	size_t read_args_num   = 0;
	TreeNode_t node_data   = "";
                                                  
	DO_AND_CHECK_SSCANF(read_args_num == 1, "%s");

	if(node_data[0] == '(' && current_str_len == 1)
	{
		DO_AND_CHECK_SSCANF(read_args_num == 1, " \"%[^\"]\"");

		TreeNode* new_node = OpNew(tree, &node_data);
		SET_ERROR_AND_RET_IF_NEEDED(new_node, NODE_CALLOC_ERROR);

		*node_ptr = new_node;

		read_ch_count += ReadNode(tree, &(*node_ptr)->left,  tree_text_repr + read_ch_count, errors_ptr);
		read_ch_count += ReadNode(tree, &(*node_ptr)->right, tree_text_repr + read_ch_count, errors_ptr);
		
		DO_AND_CHECK_SSCANF(read_args_num && node_data[0] == ')' && current_str_len == 1, "%s");
	}
	else
		SET_ERROR_AND_RET_IF_NEEDED((strcmp(node_data, EMPTY_NODE) == 0), TREE_STR_INVALID);

	return read_ch_count;
}

#undef DO_AND_CHECK_SSCANF
#undef SET_ERROR_AND_RET_IF_NEEDED

/*
* This Function return true recursively, when it finds elem which properties we are collecting 
*/
bool FillElemPropertyStack(Akinator* akinator, TreeNode* node, Stack* property_stack, char* elem_to_search)
{
	assert(property_stack != nullptr);
	assert(elem_to_search != nullptr);

	if(!node)
		return false;

	if (strcmp(akinator->tree.ElemPrinter(&node->elem), elem_to_search) == 0)
		return true;

	if (FillElemPropertyStack(akinator, node->left, property_stack, elem_to_search))
	{
		StackPush(property_stack, GOT_PROP);
		return true;
	}
	
	if (FillElemPropertyStack(akinator, node->right, property_stack, elem_to_search))
	{
		StackPush(property_stack, DOESNT_GOT_PROP);
		return true;
	}

	return false;
}

void AkinatorGiveDefinition(Akinator* akinator, char* node_elem)
{
	assert(akinator  != nullptr);
	assert(node_elem != nullptr);

	txSpeak("\v%s properties:", node_elem);

	if (FillElemPropertyStack(akinator, akinator->tree.root, &akinator->node_properties, node_elem))
	{
		PropertyPresense property_presence = {};
		TreeNode* current_node = akinator->tree.root;

		size_t property_stack_size = akinator->node_properties.size;
		for(int i = 0; i < property_stack_size; i++)
		{
			StackPop(&akinator->node_properties, &property_presence);
			if (property_presence == GOT_PROP)
			{
				txSpeak("\v\n%s", current_node->elem);
				current_node = current_node->left;
			}
			else if (property_presence == DOESNT_GOT_PROP)
			{
				txSpeak("\v\nnot %s", current_node->elem);
				current_node = current_node->right;
			}
			else
				assert(!"0");
		}
	}
	else
	{
		txSpeak("\vCan't find this person in my Database :---(");
	}
}

void AkinatorCompare(Akinator* akinator, char* node_elem1, char* node_elem2)
{
	assert(akinator   != nullptr);
	assert(node_elem1 != nullptr);
	assert(node_elem2 != nullptr);

	AkinatorGiveDefinition(akinator, node_elem1);
	txSpeak("\v but,\n");
	AkinatorGiveDefinition(akinator, node_elem2);
}

char GetUserInput(const char* correct_answers, size_t arr_size)
{
	assert(correct_answers != nullptr);

	size_t valid_inputs_num = 0;
	char user_input = 0;

	while (true)
	{
		char ch = 0;
		user_input = getchar();

		if((ch = getchar()) != '\n' && ch != EOF)  // ???
		{
			while((ch = getchar()) != '\n' && ch != EOF);
			continue;
		}

		for (int i = 0; i < arr_size; i++)
		{
			if (user_input == correct_answers[i])
				return user_input;
		}
	}
}

char AkinatorGetGameMode(Akinator* akinator)
{
	assert(akinator != nullptr);

	txSpeak("\vHey BR! Today u will play DIMA! (Delicious Incredible Magestic Akinator) \n"
		"What mode do you want to play?\n"
		"Modes: Guessing (g), Compare (c), Defining (d)):\n");

	return GetUserInput(GAME_MODES, ARR_LEN(GAME_MODES));
}

void AkinatorAddUserNode(Akinator* akinator, TreeNode* node)
{
	assert(node != nullptr);

	TreeNode_t new_node_elem      = "";
	TreeNode_t new_node_elem_diff = "";

	size_t valid_inputs_num = 0;

	txSpeak("\vWhat's on your mind?\n");
	SSCANF_WITH_CHECK(" %[^\n]", new_node_elem);

	txSpeak("\vHow %s different from %s?\n", akinator->tree.ElemPrinter(&new_node_elem), akinator->tree.ElemPrinter(&node->elem));
	
	SSCANF_WITH_CHECK(" %[^\n]", new_node_elem_diff);

	txSpeak("\vOkay I'll remember it!!!\n");

	node->right = OpNew(&akinator->tree, &node->elem);
	strcpy(node->elem, new_node_elem_diff);
	node->left = OpNew(&akinator->tree, &new_node_elem);
}

void AkinatorSaveResults(Akinator* akinator)
{
	assert(akinator != nullptr);

	FILE* akinator_tree_file = fopen(akinator->text_info.file_name, "w");
	
	PrintNodePreOrder(&akinator->tree, akinator->tree.root, akinator_tree_file);

	fclose(akinator_tree_file);
}

void AkinatorGuess(Akinator* akinator)
{
	TreeNode* current_node = akinator->tree.root;
	char user_input = 0;

	while (current_node->left && current_node->right)
	{
		txSpeak("\v%s? (y, n)\n", akinator->tree.ElemPrinter(&current_node->elem));

		user_input = GetUserInput(YES_NO_USER_INPUTS, ARR_LEN(YES_NO_USER_INPUTS));		

		if (user_input == YES_INPUT)
			current_node = current_node->left;
		else if (user_input == NO_INPUT)
			current_node = current_node->right;
		else
			assert(!"0");
	}
		
	txSpeak("\v%s? (y, n)\n", akinator->tree.ElemPrinter(&current_node->elem));
	user_input = GetUserInput(YES_NO_USER_INPUTS, ARR_LEN(YES_NO_USER_INPUTS));		

	if (user_input == YES_INPUT)
		txSpeak("\vSo, I guessed it again, thanks to my incredible creator Dimas!\n");
	else if (user_input == NO_INPUT)
	{
		AkinatorAddUserNode(akinator, current_node);

		txSpeak("\vDo you want to save results in Data Base? (y, n)\n");

		user_input = GetUserInput(YES_NO_USER_INPUTS, ARR_LEN(YES_NO_USER_INPUTS));

		if (user_input == YES_INPUT)
			AkinatorSaveResults(akinator);
	}
	else
		assert(!"0");
}

unsigned AkinatorPerformGame(Akinator* akinator)
{
	ERROR_PROCESSING(akinator, AkinatorVerifier, AkinatorDump, AkinatorDtor);
		
	TreeNode_t tree_node1_elem = {};
	TreeNode_t tree_node2_elem = {};

	switch(AkinatorGetGameMode(akinator))
	{
	case GUESS_GAME_MODE:
		AkinatorGuess(akinator);
		break;
	case COMP_GAME_MODE:

		txSpeak("\vName first person to compare:\n");
		SSCANF_WITH_CHECK(" %[^\n]", tree_node1_elem);
		txSpeak("\vName second person to compare:\n");
		SSCANF_WITH_CHECK(" %[^\n]", tree_node2_elem);

		AkinatorCompare(akinator, tree_node1_elem, tree_node2_elem);
		break;
	case DEF_GAME_MODE:
		txSpeak("\vName person to define:\n");
		SSCANF_WITH_CHECK(" %[^\n]", tree_node1_elem);

		AkinatorGiveDefinition(akinator, tree_node1_elem);
		break;
	default:
		assert(!"0");
	}
	
	return 0;
}

void AkinatorElemCtor(TreeNode_t* new_elem, TreeNode_t* new_data)
{
	TreeNode_t result_elem = {};
	snprintf(result_elem, sizeof(result_elem), "\"%s\"", *new_data);

	strcpy(*new_elem, result_elem);
} 

void AkinatorElemDtor(TreeNode_t* elem_to_delete)
{
	;
}

char* AkinatorElemPrinter(const TreeNode_t* elem_to_print)
{
	assert(elem_to_print != nullptr);

	static TreeNode_t result_elem = "";

	size_t valid_inputs_num = sscanf(*elem_to_print, " \"%[^\"]\"", result_elem);

	if(!valid_inputs_num)
		return (char*)(*elem_to_print);

	return (char*)result_elem;
}

unsigned AkinatorCtor(Akinator* akinator)
{
	assert(akinator != nullptr);

	fopen_s(&akinator->logger, "akinator_logger.txt", "w");
	fopen_s(&akinator->graph,  "akinator_graph.gv",   "w");

	if (TextInfoCtor(&akinator->text_info, "source.txt"))
		akinator->errors &= TEXT_INFO_ERROR;

	if (StackCtor(&akinator->node_properties))
		akinator->errors &= PROPERTY_STACK_ERROR;

	TreeCtor(&akinator->tree, AkinatorElemCtor, AkinatorElemDtor, AkinatorElemPrinter);
	
	unsigned errors = 0;
	ReadNode(&akinator->tree, &akinator->tree.root, akinator->text_info.text, &errors);
	if(errors)
	{
		akinator->errors &= AKINATOR_TREE_ERROR;
		ReadNode(&akinator->tree, &akinator->tree.root, "( \"NoName\" nil nil )", &errors);
	}

	ERROR_PROCESSING(akinator, AkinatorVerifier, AkinatorDump, AkinatorDtor);

	return 0;
}

unsigned AkinatorDtor(Akinator* akinator)
{
	assert(akinator != nullptr);

	if (!(akinator->errors & AKINATOR_DELETED))
	{
		fclose(akinator->logger);
		fclose(akinator->graph);
		
		unsigned destructor_errors = 0;
		if (StackDtor(&akinator->node_properties))
			destructor_errors |= PROPERTY_STACK_ERROR;

		TreeDtor(&akinator->tree);

		akinator->errors |= AKINATOR_DELETED;

		return destructor_errors;
	}
	else
		return AKINATOR_DELETED;
}