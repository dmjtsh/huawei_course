#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "akinator.h"
#include "error_processing.h"

Node* OpNew(char* data)
{
	if (!data) return 0;

	Node* node = {};
	node = (Node*)calloc(1, sizeof(Node));
	if(!node) return 0;

	strcpy(node->elem, data);

	return node;
}

void OpDelete(Node* node)
{
	if (!node) return;

	free(node);
}

void PrintNode(const Node* node, FILE* logger)
{
	if (!node) { fprintf(logger, "nil "); return; }
	
	fprintf(logger, "( ");

	fprintf(logger, "%s ", node->elem);
	
	PrintNode(node->left, logger);
	
	PrintNode(node->right, logger);
	
	fprintf(logger, ") ");
}

void AkinatorDump(Akinator* akinator, FILE* logger)
{
	assert(akinator != NULL);
	assert(logger != NULL);

	static size_t num_of_call = 1;
	fprintf(logger, 
	"=======================================\n"
	"AKINATOR DUMP CALL #%zu\n", num_of_call);
	if (akinator->errors)
	{
		fprintf(logger,
		"-------------ERRORS------------\n");
		if (akinator->errors & AKINATOR_PTR_NULL)
		{
			fprintf(logger, "AKINATOR POINTER IS NULL\n");
			return;
		}
		if (akinator->errors & AKINATOR_LOGER_ERROR)     fprintf(logger, "AKINATOR LOGER ERROR\n");
		if (akinator->errors & ROOT_PTR_NULL)            fprintf(logger, "ROOT PTR NULL\n");
        if (akinator->errors & NODE_CALLOC_ERROR)        fprintf(logger,  "NODE CALLOC ERROR\n");
        if (akinator->errors & AKINATOR_BAD_SIZE)        fprintf(logger, "AKINATOR SIZE TOO BIG\n");
        if (akinator->errors & AKINATOR_GRAPH_ERROR)     fprintf(logger, "AKINATOR GRAPH ERROR\n");
		if (akinator->errors & AKINATOR_TEXT_INFO_ERROR) fprintf(logger, "AKINATOR TEXT INFO ERROR\n");

		fprintf(logger,
		"----------END_OF_ERRORS--------\n");
	}
	else
	{
		fprintf(logger,
		"------------NO_ERRORS----------\n");

		fprintf(logger, "Akinator Tree:\n\n");

		PrintNode(akinator->root, logger);

		fprintf(logger, "\n\n");
	}
	
	fprintf(logger,
	"=======================================\n\n");

	num_of_call++;
}							  


unsigned AkinatorVerifier(Akinator* akinator)
{
	if (!akinator)
		return AKINATOR_PTR_NULL;

	CHECK_ERROR(akinator, akinator->logger == NULL,            AKINATOR_LOGER_ERROR)
	CHECK_ERROR(akinator, akinator->logger == NULL,            AKINATOR_GRAPH_ERROR)
	CHECK_ERROR(akinator, akinator->size >= AKINATOR_MAX_SIZE, AKINATOR_BAD_SIZE)
	CHECK_ERROR(akinator, !akinator->text_info.is_okay       , AKINATOR_TEXT_INFO_ERROR)

	return akinator->errors;
}

void DestroyTree(Node** node_ptr)
{
	if (!node_ptr || !*node_ptr)
		return;

	if((*node_ptr)->left)
		DestroyTree(&(*node_ptr)->left);
	if((*node_ptr)->right)
		DestroyTree(&(*node_ptr)->right);

	free(*node_ptr);

	*node_ptr = 0;
}

#define DESTROY_TREE_AND_RET(node_ptr) \
	*errors_ptr &= TREE_STR_INVALID;   \
	DestroyTree(node_ptr);             \
	return 0;                          \

#define DO_AND_CHECK_SSCANF(check_cond)                                                             \
	read_args_num = sscanf(tree_text_repr + read_ch_count, "%s%n", node_data, &current_word_len);   \
	read_ch_count += current_word_len + 1;                                                          \
	if (!(check_cond))                                                                              \
	{                                                                                               \
		DESTROY_TREE_AND_RET(node_ptr)                                                              \
	}                                                                                              

size_t ReadNode(Node** node_ptr, const char* tree_text_repr, unsigned* errors_ptr)
{
	if (!errors_ptr || !tree_text_repr)
		return 0;

	if(*errors_ptr)
	{
		DESTROY_TREE_AND_RET(node_ptr)
	}

	size_t read_ch_count     = 0;
	size_t current_word_len = 0;
	size_t read_args_num     = 0;
	char node_data[MAX_NODE_STR_LEN] = "";

	DO_AND_CHECK_SSCANF(read_args_num == 1)

	if(node_data[0] == '(' && current_word_len == 1)
	{
		DO_AND_CHECK_SSCANF(read_args_num == 1)

		Node* new_node = OpNew(node_data);
		*node_ptr = new_node;

		read_ch_count += ReadNode(&(*node_ptr)->left,  tree_text_repr + read_ch_count, errors_ptr);
		read_ch_count += ReadNode(&(*node_ptr)->right, tree_text_repr + read_ch_count, errors_ptr);
		
		DO_AND_CHECK_SSCANF(read_args_num && node_data[0] == ')' && current_word_len == 1)
	}
	else
	{
		if (!(strcmp(node_data,"nil") == 0))
		{
			DESTROY_TREE_AND_RET(node_ptr)
		}
	}

	return read_ch_count;
}

#undef DESTROY_TREE_AND_RET
#undef DO_AND_CHECK_SSCANF

char GetUserInput(char* correct_answers, size_t arr_size)
{
	size_t valid_inputs_num = 0;
	char         user_input = 0;

	while (true)
	{
		valid_inputs_num = scanf("%c", &user_input);
		
		if(valid_inputs_num == 1)
		{
			for (int i = 0; i < arr_size; i++)
			{
				if (user_input == correct_answers[i])
					return user_input;
			}
		}
	}
}

bool AkinatorDoIntro(Akinator* akinator)
{
	printf("Hey BR! Today u will play DIMA! (Delicious Incredible Magestic Akinator) \n"
		   "Do you want to play? (y, n)\n");
	
	char correct_answers[] = {YES_INPUT, NO_INPUT};
	char        user_input = 0;

	user_input = GetUserInput(correct_answers, sizeof(correct_answers) / sizeof(correct_answers[0]));

	if (user_input == YES_INPUT)
	{
		printf("Okeysi! Lets start the game!\n\n");
		return true;
	}
	else if (user_input == NO_INPUT)
	{
		printf("It so sad that Steve Jobs died from ligma...\n\n");
		return false;
	}
	else
		assert(0);
}

void FormatStringWithUnderscores(char* str, size_t str_len)
{
	for(int i = 0; i < str_len ; i++)
	{
		if (str[i] == ' ' || str[i] == '\t')
			str[i] = '_';
	}
}

void PrintFormatedString(char* str)
{
	while (*str != '\0') 
	{
		if (*str != '_') 
			putchar(*str);
		else
			putchar(' ');
		str++;
	}
}

void AkinatorAddUserNode(Node* node)
{
	char new_node_elem[MAX_NODE_STR_LEN]      = "";
	char new_node_elem_diff[MAX_NODE_STR_LEN] = "";
	size_t user_input_len   = 0;
	size_t valid_inputs_num = 0;

	printf("What's on your mind?\n");

	do
	{
		valid_inputs_num = scanf(" %[^\n]%n", new_node_elem, &user_input_len);
	}
	while(valid_inputs_num != 1);

	printf("How %s different from %s?\n", new_node_elem, node->elem);
	
	do
	{
		valid_inputs_num = scanf(" %[^\n]%n", new_node_elem_diff, &user_input_len);
	}
	while(valid_inputs_num != 1);

	printf("Okay I'll remember it!!!\n");

	FormatStringWithUnderscores(new_node_elem, user_input_len);
	FormatStringWithUnderscores(new_node_elem_diff, user_input_len);

	node->right  = OpNew(node->elem);
	strcpy(node->elem, new_node_elem_diff);
	node->left = OpNew(new_node_elem);
}

void AkinatorCelebrateVictory()
{
	printf("So, I won again, thanks to my incredible creator Dimas!\n");
}

void AkinatorSaveResults(Akinator* akinator)
{
	FILE* akinator_tree_file = fopen(akinator->text_info.file_name, "w");
	
	PrintNode(akinator->root, akinator_tree_file);

	fclose(akinator_tree_file);
}

unsigned AkinatorPerformGame(Akinator* akinator)
{
	ERROR_PROCESSING(akinator, AkinatorVerifier, AkinatorDump, AkinatorDtor);
		
	char        user_input      = 0;
	char correct_answers[]      = {YES_INPUT, NO_INPUT};
	size_t correct_answers_size = sizeof(correct_answers) / sizeof(correct_answers[0]);

	if (AkinatorDoIntro(akinator))
	{
		Node* current_node = akinator->root;
	
		while (current_node->left && current_node->right)
		{
			PrintFormatedString(current_node->elem);
			printf("? (y, n)\n");

			user_input = GetUserInput(correct_answers, correct_answers_size);		

			if (user_input == YES_INPUT)
				current_node = current_node->left;
			else if (user_input == NO_INPUT)
				current_node = current_node->right;
			else
				assert(0);
		}
		
		PrintFormatedString(current_node->elem);
		printf(" (y, n)\n");

		user_input = GetUserInput(correct_answers, correct_answers_size);		

		if (user_input == YES_INPUT)
			AkinatorCelebrateVictory();
		else if (user_input == NO_INPUT)
		{
			AkinatorAddUserNode(current_node);

			printf("Do you want to save results in Data Base? (y, n)\n");

			user_input = GetUserInput(correct_answers, correct_answers_size);

			if (user_input == YES_INPUT)
				AkinatorSaveResults(akinator);
		}
		else
			assert(0);
	}
	else
		return 0;
}

unsigned AkinatorCtor(Akinator* akinator)
{
	if(!akinator)
		return AKINATOR_PTR_NULL;

	fopen_s(&akinator->logger, "akinator_logger.txt", "w");

	fopen_s(&akinator->graph,  "akinator_graph.gv",   "w");

	if (TextInfoCtor(&akinator->text_info, "source.txt"))
		akinator->errors &= TEXT_INFO_ERROR;

    ReadNode(&akinator->root, akinator->text_info.text, &akinator->errors);

	if(!akinator->root)
		ReadNode(&akinator->root, "( Noname nil nil )", &akinator->errors);

	ERROR_PROCESSING(akinator, AkinatorVerifier, AkinatorDump, AkinatorDtor)

	return 0;
}

unsigned AkinatorDtor(Akinator* akinator)
{
	if (!akinator)
		return AKINATOR_PTR_NULL;

	DestroyTree(&akinator->root);

	fclose(akinator->logger);

	fclose(akinator->graph);

	return 0;
}