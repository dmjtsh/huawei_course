#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "differentiator.h"
#include "differentiator_errors.h"
#include "DimasLIB/DimasUtilities/error_processing.h"

void SetNum(TreeNode_t* data, double num)
{
	assert(data != nullptr);

	data->type     = NUM;
	data->elem.num = num;
}

void SetVar(TreeNode_t* data, char var)
{
	assert(data != nullptr);

	data->type          = VAR;
	data->elem.var.name = var;
}

bool TrySetOper(TreeNode_t* data, char* oper_str)
{
	assert(data     != nullptr);
	assert(oper_str != nullptr);

	bool is_oper_found = false;

	#define CMD_DEF(value, designation, ...)                  \
	if(!is_oper_found && strcmp(designation, oper_str) == 0)  \
	{                                                         \
		is_oper_found = true;			                      \
		data->elem.oper = value;                              \
		data->type = OPER;                                    \
	}
	
	#include "cmds_defs.h"

	#undef CMD_DEF

	return is_oper_found;
}

bool TrySetStrElem(TreeNode_t* data, char* str, size_t str_len)
{
	assert(data != nullptr);
	assert(str  != nullptr);


	if (TrySetOper(data, str))
		return true;
	else if( str_len == 1)
	{
		SetVar(data, *str);
		return true;
	}
	else
		return false;
}

// TODO: ƒобавить более естественные названи€ дл€ макросов
#define SET_ERROR_AND_RET_IF_NEEDED(cond, error)     \
	do                                               \
	{												 \
		if(!(cond))									 \
		{											 \
			*errors_ptr |= error;					 \
			return 0;								 \
		}											 \
	} while(0)

#define DO_SSCANF(scanf_format, var_to_write_in)																   \
	do																											   \
	{																											   \
		read_args_num = sscanf(tree_text_repr + read_ch_count, scanf_format "%n", var_to_write_in, &curr_str_len); \
		read_ch_count += curr_str_len + 1;                                                                         \
	} while(0)

#define DO_AND_CHECK_SSCANF(scanf_format, var_to_write_in, check_cond)                                          \
	do																										    \
	{																										    \
		DO_SSCANF(scanf_format, var_to_write_in);																\
		SET_ERROR_AND_RET_IF_NEEDED(check_cond, DIFF_FILE_INVALID_SYNTAX);                                      \
	} while(0)

size_t ReadNode(Tree* tree, TreeNode** node_ptr, const char* tree_text_repr, unsigned* errors_ptr)
{
	assert(node_ptr       != nullptr);
	assert(tree_text_repr != nullptr);
	assert(errors_ptr     != nullptr);         

	if(*errors_ptr) return 0;       

	size_t read_ch_count = 0;
	size_t curr_str_len  = 0;
	size_t read_args_num = 0;

	char       str_node_data[MAX_OPER_LEN] = "";
	double     num_node_data               = 0;
	TreeNode_t node_data = {};
	
	// OPEN BRACKET CHECK
	DO_AND_CHECK_SSCANF("%s", str_node_data, read_args_num == 1);

	if(str_node_data[0] == '(' && curr_str_len == 1)
	{
		// LEFT CHILD
		TreeNode* left_child = {};
		read_ch_count += ReadNode(tree, &left_child,  tree_text_repr + read_ch_count, errors_ptr);

		DO_SSCANF("%lf", &num_node_data);
		if(read_args_num != 1)
		{
			read_ch_count -= curr_str_len + 1; // GOING BACK AFTER PREVIOS SSCANF 

			DO_AND_CHECK_SSCANF("%s", str_node_data, read_args_num == 1 && TrySetStrElem(&node_data, str_node_data, curr_str_len));
		}
		else
			SetNum(&node_data, num_node_data);
		
		TreeNode* new_node = OpNew(tree, &node_data);
		SET_ERROR_AND_RET_IF_NEEDED(new_node, NODE_CALLOC_ERROR);
		*node_ptr = new_node;
		
		// RIGHT CHILD
		TreeNode* right_child = {};
		read_ch_count += ReadNode(tree, &right_child, tree_text_repr + read_ch_count, errors_ptr);
		
		(*node_ptr)->left  = left_child;
		(*node_ptr)->right = right_child;

		// CLOSE BRACKET CHECK
		DO_AND_CHECK_SSCANF("%s", str_node_data, read_args_num && str_node_data[0] == ')' && curr_str_len == 1);
	}
	else
		SET_ERROR_AND_RET_IF_NEEDED((strcmp(str_node_data, EMPTY_NODE) == 0), DIFF_FILE_INVALID_SYNTAX);

	return read_ch_count;
}

#undef DO_AND_CHECK_SSCANF
#undef DO_SSCANF_AND_MOVE
#undef SET_ERROR_AND_RET_IF_NEEDED

#define NODE_TYPE     node->node_elem.type
#define NODE_NUM      node->node_elem.elem.num
#define NODE_OPER     node->node_elem.elem.oper
#define OPER_PRIORITY node->node_elem.elem.oper.priority

#define NEW_DATA_OPER_NUM new_data.elem.oper.oper_num

// TODO: ADD binary Ops
double Eval(const TreeNode* node)
{
	if (!node)
		return 0;

	if (NODE_TYPE == NUM)
		return NODE_NUM;

	double left  = Eval(node->left);
	double right = Eval(node->right);

	switch(NODE_OPER)
	{
		case ADD:
			return left + right;
		case SUB:
			return left - right;
		case MUL:
			return left * right;
		case DIV:
			return left / right;
		case POW:
			return pow(left, right);
		default:
			assert(0);
	}
}

// TODO: ‘ункцию перекинуть в tree.h
TreeNode* CreateNode(Tree* tree, TreeNode_t* data, TreeNode** node, TreeNode* left_node, TreeNode* right_node)
{
	assert(tree != nullptr);
	assert(node != nullptr);

	DestroyNode(tree, *node);

	*node = OpNew(tree, data);

	(*node)->left  = left_node;
	(*node)->right = right_node;

	return *node;
}

TreeNode* CopyNode(Tree* tree, TreeNode* node)
{
	assert(tree != nullptr);

	if(!node)
		return nullptr;

	TreeNode* new_node = {};
	new_node = OpNew(tree, &node->node_elem);
	
	new_node->left  = CopyNode(tree, node->left);
	new_node->right = CopyNode(tree, node->right);

	return new_node;
}

TreeNode* CreateOperNode(Differentiator* diff, Operation oper, TreeNode* left_node, TreeNode* right_node)
{
	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type      = OPER;
	new_data.elem.oper = oper;

	return CreateNode(&diff->tree, &new_data, &new_node, left_node, right_node);
}

TreeNode* CreateVarNode(Differentiator* diff, char var_name)
{
	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type          = VAR;
	new_data.elem.var.name = var_name;

	return CreateNode(&diff->tree, &new_data, &new_node, nullptr, nullptr);
}

TreeNode* CreateNumNode(Differentiator* diff, double num)
{
	TreeNode*  new_node = {};
	TreeNode_t new_data = {};
	
	new_data.type     = NUM;       
	new_data.elem.num = num;

	return CreateNode(&diff->tree, &new_data, &new_node, nullptr, nullptr);
}

bool IsNodeNum(TreeNode* node)
{
	if(!node)
		return true;

	if (NODE_TYPE == VAR)
		return false;

	if (IsNodeNum(node->left) && IsNodeNum(node->right))
		return true;
	else
		return false;
}

// TODO: во всех фукнци€х вывода const расставить

#define CREATE_OPER(operation, left, right) CreateOperNode(diff, operation, left, right)

#define CREATE_NUM(number) CreateNumNode(diff, number)

#define DIFF(elem_to_diff) Differentiate(diff, elem_to_diff)
#define COPY(elem_to_copy) CopyNode(&diff->tree, elem_to_copy)

#define _ADD(left_child, right_child) CREATE_OPER(ADD, left_child, right_child)
#define _SUB(left_child, right_child) CREATE_OPER(SUB, left_child, right_child)
#define _MUL(left_child, right_child) CREATE_OPER(MUL, left_child, right_child)
#define _DIV(left_child, right_child) CREATE_OPER(DIV, left_child, right_child)
#define _POW(left_child, right_child) CREATE_OPER(POW, left_child, right_child)

// TODO: ѕроверить везде ли передаютс€ указатели 
TreeNode* Differentiate(Differentiator* diff, TreeNode* node)
{
	assert(diff != nullptr);

	if(node == nullptr)
		return nullptr;
	
	TreeNode*  new_node = {};
	TreeNode_t new_data = {};

	if (NODE_TYPE == NUM)
		return CREATE_NUM(0);
	if (NODE_TYPE == VAR)
		return CREATE_NUM(1);
	
	switch (NODE_OPER)
	{
		case ADD:
			new_node = _ADD(DIFF(node->left), DIFF(node->right));
			break;
		case SUB:
			new_node = _SUB(DIFF(node->left), DIFF(node->right));
		    break;
		case MUL:
			new_node = _ADD(_MUL(DIFF(node->left), COPY(node->right)), _MUL(COPY(node->left), DIFF(node->right)));
			break;
		case DIV:
			new_node = _DIV(_SUB(_MUL(DIFF(node->left), COPY(node->right)), _MUL(COPY(node->left), DIFF(node->right))), _MUL(node->right, COPY(node->right)));
			break;
		case POW:
			if (IsNodeNum(node->right) && IsNodeNum(node->left))
				new_node = CREATE_NUM(0);
			else if (IsNodeNum(node->right))
				new_node = _MUL(_MUL(COPY(node->right), _POW(COPY(node->left), _SUB(COPY(node->right), CREATE_NUM(1)))), DIFF(node->left));
			else
			{
				assert(0);
			}
			node->right;
			break;
			// TODO: добавить остальные операции
		default: assert(0);
	}

	DestroyNode(&diff->tree, node);
	
	diff->tree.root = new_node;

	return new_node;
}

OperPriority GetOperPriority(Operation oper)
{
	OperPriority priority = LAST_PRIORITY;

	#define CMD_DEF(value, designation, oper_priority) \
		if(oper == value)                              \
			priority = oper_priority;

	#include "cmds_defs.h"

	#undef CMD_DEF

	return priority;
}

char* GetOperDesignation(Operation oper)
{
	static char design[MAX_OPER_LEN] = "";

	#define CMD_DEF(value, designation, ...) \
		if(oper == value)                    \
			strcpy(design, designation);

	#include "cmds_defs.h"

	#undef CMD_DEF

	return design;
}

bool IsBracketsNeeded(TreeElemType node_type, OperPriority parent_node_priority, OperPriority child_node_priority)
{
	return node_type == OPER && parent_node_priority > child_node_priority;
}

// TODO: do tree verifier
void PrintLatexDiffNode(FILE* file_to_print, Differentiator* diff, TreeNode* node, TreeNode* parent_node)
{
	assert(diff != nullptr);

	if(node == nullptr)
		return;

	if (NODE_TYPE == NUM || NODE_TYPE == VAR)
	{
		fprintf(file_to_print, diff->tree.ElemPrinter(&node->node_elem));
		return;
	}

	assert(NODE_TYPE == OPER); // TODO: do this normal

	OperPriority node_priority        = GetOperPriority(NODE_OPER);
	OperPriority parent_node_priority = LAST_PRIORITY;
	if (parent_node)
		parent_node_priority = GetOperPriority(parent_node->node_elem.elem.oper);

	if (IsBracketsNeeded(NODE_TYPE, parent_node_priority, node_priority))
		fprintf(file_to_print, "{");

	switch(NODE_OPER)
	{
	
		#define CMD_DEF(value, designation, priority, latex_printing_code, ...) \
		case value:                                                             \
			latex_printing_code;                                                \
			break;

		#include "cmds_defs.h"

		default:
			assert(0);
	}
	
	#undef CMD_DEF

	if (IsBracketsNeeded(NODE_TYPE, parent_node_priority, node_priority))
		fprintf(file_to_print, "}");
}

char* DifferentiatorElemPrinter(const TreeNode_t* elem_to_print)
{
	assert(elem_to_print != nullptr);

	static char str_to_output[MAX_COMMAND_LENGTH] = "";

	if (elem_to_print->type == NUM)
	{
		if (IsDoubleInt(elem_to_print->elem.num))
			sprintf(str_to_output, "%d", (int)elem_to_print->elem.num);
		else
			sprintf(str_to_output, "%lf", elem_to_print->elem.num);
	}
	else if (elem_to_print->type == VAR)
		sprintf(str_to_output, "%c", elem_to_print->elem.var.name);
	else if (elem_to_print->type == OPER)
		return GetOperDesignation(elem_to_print->elem.oper);
	else
		assert(0);
	
	return str_to_output;
}

void DifferentiatorElemCtor(TreeNode_t* new_elem, TreeNode_t* new_data)
{
	assert(new_elem != nullptr);
	assert(new_data != nullptr);

	*new_elem = *new_data;
} 

void DifferentiatorElemDtor(TreeNode_t* elem_to_delete)
{
	assert(elem_to_delete != nullptr);
}

unsigned DifferentiatorCtor(Differentiator* diff)
{
	assert(diff != nullptr);

	if (TextInfoCtor(&diff->text_info, "source.txt"))
		diff->errors |= TEXT_INFO_ERROR;

	TreeCtor(&diff->tree, DifferentiatorElemCtor, DifferentiatorElemDtor, DifferentiatorElemPrinter);

	unsigned read_node_errors = 0;
	ReadNode(&diff->tree, &diff->tree.root, diff->text_info.text, &read_node_errors);

	if (read_node_errors)
		ReadNode(&diff->tree, &diff->tree.root, "( . 0 . )", &read_node_errors); 
	
	assert(&diff->tree.root != nullptr);

	return 0;
}

unsigned DifferentiatorDtor(Differentiator* diff)
{
	assert(diff != nullptr);

	if (!(diff->errors & DIFF_DELETED))
	{
		TreeDtor(&diff->tree);

		diff->errors |= DIFF_DELETED;

		return 0;
	}
	else
		return DIFF_DELETED;
}