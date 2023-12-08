#define DIFF(elem_to_diff) DifferentiateNode(expr_tree, elem_to_diff, var)
#define COPY(elem_to_copy) CopyNode(expr_tree, elem_to_copy)

#define LEFT                         node->left
#define RIGHT                        node->right
#define _ADD(left_node, right_node)  CREATE_OPER(ADD, left_node, right_node)
#define _SUB(left_node, right_node)  CREATE_OPER(SUB, left_node, right_node)
#define _MUL(left_node, right_node)  CREATE_OPER(MUL, left_node, right_node)
#define _DIV(left_node, right_node)  CREATE_OPER(DIV, left_node, right_node)
#define _POW(left_node, right_node)  CREATE_OPER(POW, left_node, right_node)
#define _SIN(_node)                  CREATE_OPER(SIN, nullptr,    _node)
#define _COS(_node)                  CREATE_OPER(COS, nullptr,    _node)
#define _LN(_node)                   CREATE_OPER(LN,  nullptr,    _node)

#define PRINT_BINARY_OPER(oper)								        \
	do															    \
	{															    \
		PrintLatexNode(expr_tree, LEFT, node);						\
		DO_LATEX_PRINTF(oper);                                      \
		PrintLatexNode(expr_tree, RIGHT, node);						\
	} while(0)

#define PRINT_UNARY_OPER(oper)                                      \
	do                                                              \
	{									                            \
		DO_LATEX_PRINTF( oper "(" );                                \
		PrintLatexNode(expr_tree, RIGHT, node);                     \
		DO_LATEX_PRINTF(")");								        \
	} while(0)

/*
* ALL CMD_DEF HAS THIS STRUCTURE:
* CMD_DEF(name, user designation, priority, latex printing code, differentiate_code);
*/

OPER_DEF(MUL, "*",   SECOND_PRIORITY, TWO_ARGS,
	PRINT_BINARY_OPER("*"),

	//  y' = F(x) * (G(x))' + (F(x))' * (G(x))
	new_node = _ADD(_MUL(DIFF(LEFT), COPY(RIGHT)), _MUL(COPY(LEFT), DIFF(RIGHT)))
)
			 	    
OPER_DEF(DIV, "/",   SECOND_PRIORITY, TWO_ARGS,
	DO_LATEX_PRINTF("\\frac{");

	PrintLatexNode(expr_tree, LEFT, node);
	DO_LATEX_PRINTF("}{");
	PrintLatexNode(expr_tree, RIGHT, node);

	DO_LATEX_PRINTF("}");,

	//  y' = (F(x) * (G(x))' - (F(x))' * (G(x))) / (G(x))^2 
	new_node = _DIV(_SUB(_MUL(DIFF(LEFT), COPY(RIGHT)), _MUL(COPY(LEFT), DIFF(RIGHT))), _MUL(COPY(RIGHT), COPY(RIGHT)))
)
			 	    
OPER_DEF(ADD, "+", THIRD_PRIORITY, TWO_ARGS,
	PRINT_BINARY_OPER("+"),
	
	//  y' = (F(x))' + (G(x))'
	new_node = _ADD(DIFF(LEFT), DIFF(RIGHT))
)
			 	    
OPER_DEF(SUB, "-",   THIRD_PRIORITY, TWO_ARGS,
	PRINT_BINARY_OPER("-"),
	
	// y' = (F(x))' - (G(x))'
	new_node = _SUB(DIFF(LEFT), DIFF(RIGHT))
)
             
OPER_DEF(POW, "^",   FIRST_PRIORITY, TWO_ARGS,
	PRINT_BINARY_OPER("^"),
	
	// num1^num2 <- CASE
	if (ContainsNumsOnly(RIGHT) && ContainsNumsOnly(LEFT))
		// y' = 0
		new_node = CREATE_NUM(0);
	// F(x)^num  <- CASE
	else if (ContainsNumsOnly(RIGHT))
		// y' = num * F(x)^(num - 1) * (x') 
		new_node = _MUL(_MUL(COPY(RIGHT), _POW(COPY(LEFT), _SUB(COPY(RIGHT), CREATE_NUM(1)))), DIFF(LEFT));
	// F(x)^G(x) <- CASE
	else
	{	// y = F(x)^G(x)
		TreeNode* y = _POW(COPY(LEFT), COPY(RIGHT));
		// y' = y * [ (G(x))' * ln(F(x)) + G(x) * (ln(F(x)))' ] 
		new_node = _MUL(y, _ADD(_MUL(_LN(COPY(LEFT)), DIFF(RIGHT)), _MUL(DIFF(_LN(COPY(LEFT))), COPY(RIGHT))));
	}
)

OPER_DEF(COS, "cos", FIRST_PRIORITY, ONE_ARG,
	PRINT_UNARY_OPER("cos"),

	// y' = sinx * (-1) * x'
	new_node = _MUL(_MUL(_SIN(COPY(RIGHT)), CREATE_NUM(-1)), DIFF(RIGHT))
)
             
OPER_DEF(SIN, "sin", FIRST_PRIORITY, ONE_ARG,
	PRINT_UNARY_OPER("sin"),
	
	// y' = cosx * x'
	new_node = _MUL(_COS(COPY(RIGHT)), DIFF(RIGHT))
)
             
OPER_DEF(LN,  "ln",  FIRST_PRIORITY, ONE_ARG,
	PRINT_UNARY_OPER("ln"),

	// y' = 1/x * x'
	new_node = _MUL(_DIV(CREATE_NUM(1), COPY(RIGHT)), DIFF(RIGHT))
)

#undef PRINT_UNARY_OPER
#undef PRINT_BINARY_OPER

/*
  
  HAPPY CODING!
 
  __0__
    ☺ <---(IM IN MEXICAN HEAD)
  --|--
 	|
   / \

*/