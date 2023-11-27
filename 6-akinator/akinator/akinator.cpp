#define _CRT_SECURE_NO_WARNINGS
#define TX_USE_SPEAK

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "TX/TXLib.h"
#include "akinator.h"
#include "akinator_errors.h"
#include "DimasLIB/DimasUtilities/error_processing.h"

/*
* This Function return true recursively, when it finds elem which properties we are collecting 
*/
bool FillElemPropertyStack(TreeNode* node, Stack* property_stack, char* elem_to_search)
{
	assert(property_stack != nullptr);
	assert(elem_to_search != nullptr);

	if(!node)
		return false;

	if (strcmp(node->elem, elem_to_search) == 0)
		return true;

	if (FillElemPropertyStack(node->left, property_stack, elem_to_search))
	{
		StackPush(property_stack, GOT_PROP);
		return true;
	}
	
	if (FillElemPropertyStack(node->right, property_stack, elem_to_search))
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

	printf("%s properties:", node_elem);
	//txSpeak("%s", node_elem);

	if (FillElemPropertyStack(akinator->tree.root, &akinator->node_properties, node_elem))
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
			else if ( property_presence == DOESNT_GOT_PROP)
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
		printf("Can't find this person in my Database :---(");
		//txSpeak("\v Can't find this person in my Database :---(");
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

		if((ch = getchar()) != '\n')  // ????
		{
			while((ch = getchar()) != '\n');
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

	printf("Hey BR! Today u will play DIMA! (Delicious Incredible Magestic Akinator) \n"
		"What mode do you want to play\n"
		"Modes: Guessing (g), Compare(c), Defining (d)):\n");
	
	//txSpeak(\v"Hey BR! Today u will play DIMA! (Delicious Incredible Magestic Akinator) \n"
	//	"What mode do you want to play\n"
	//	"Modes: Guessing (g), Compare(c), Defining (d)):\n");

	return GetUserInput(GAME_MODES, ARR_LEN(GAME_MODES));
}

void AkinatorAddUserNode(TreeNode* node)
{
	assert(node != nullptr);

	TreeNode_t new_node_elem      = "";
	TreeNode_t new_node_elem_diff = "";

	size_t valid_inputs_num = 0;

	printf("What's on your mind?\n");
	//txSpeak("\vWhat's on your mind?\n");
	SSCANF_WITH_CHECK(" %[^\n]", new_node_elem);

	printf("How %s different from %s?\n", new_node_elem, node->elem);
	//txSpeak("\vHow %s different from %s?\n", new_node_elem, node->elem);
	
	SSCANF_WITH_CHECK(" %[^\n]", new_node_elem_diff);

	printf("Okay I'll remember it!!!\n");
	//txSpeak("\vOkay I'll remember it!!!\n");

	node->right  = OpNew(node->elem);
	strcpy(node->elem, new_node_elem_diff);
	node->left = OpNew(new_node_elem);
}

void AkinatorSaveResults(Akinator* akinator)
{
	assert(akinator != nullptr);

	FILE* akinator_tree_file = fopen(akinator->text_info.file_name, "w");
	
	PrintNode(akinator->tree.root, akinator_tree_file);

	fclose(akinator_tree_file);
}

void AkinatorGuess(Akinator* akinator)
{
	TreeNode* current_node = akinator->tree.root;
	char user_input = 0;

	while (current_node->left && current_node->right)
	{
		printf("%s? (y, n)\n" , current_node->elem);
		//txSpeak("\v%s? (y, n)\n" , current_node->elem);

		user_input = GetUserInput(YES_NO_USER_INPUTS, ARR_LEN(YES_NO_USER_INPUTS));		

		if (user_input == YES_INPUT)
			current_node = current_node->left;
		else if (user_input == NO_INPUT)
			current_node = current_node->right;
		else
			assert(!"0");
	}
		
	printf("%s? (y, n)\n" , current_node->elem);
	//txSpeak("\v%s? (y, n)\n" , current_node->elem);

	user_input = GetUserInput(YES_NO_USER_INPUTS, ARR_LEN(YES_NO_USER_INPUTS));		

	if (user_input == YES_INPUT)
	{
		printf("So, I guessed it again, thanks to my incredible creator Dimas!\n");
		//txSpeak("\vSo, I guessed it again, thanks to my incredible creator Dimas!\n");
	}
	else if (user_input == NO_INPUT)
	{
		AkinatorAddUserNode(current_node);

		printf("Do you want to save results in Data Base? (y, n)\n");
		//txSpeak("\vDo you want to save results in Data Base? (y, n)\n");

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

		printf("Name first person to compare:\n");
		SSCANF_WITH_CHECK(" %[^\n]", tree_node1_elem);
		printf("Name second person to compare:\n");
		SSCANF_WITH_CHECK(" %[^\n]", tree_node2_elem);

		AkinatorCompare(akinator, tree_node1_elem, tree_node2_elem);
		break;
	case DEF_GAME_MODE:
		printf("Name person to define:\n");
		SSCANF_WITH_CHECK(" %[^\n]", tree_node1_elem);

		AkinatorGiveDefinition(akinator, tree_node1_elem);
		break;
	default:
		assert(!"0");
	}
	
	return 0;
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

	if (TreeCtor(&akinator->tree, akinator->text_info.text))
		akinator->errors &= AKINATOR_TREE_ERROR;

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