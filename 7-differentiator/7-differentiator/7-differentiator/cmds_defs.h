#define PRINT_IN_BINARY_OPER(oper)								\
	do															\
	{															\
	PrintLatexDiffNode(file_to_print, diff, node->left, node);  \
	fprintf(file_to_print, "*");                                \
	PrintLatexDiffNode(file_to_print, diff, node->right, node); \
	} while(0)

/*
* ALL CMD_DEF HAS THIS STRUCTURE
* CMD_DEF(name, user designation, priority, latex printing code);
*/

CMD_DEF(MUL, "*",   SECOND_PRIORITY, 
	PRINT_IN_BINARY_OPER(*);
)
			 	    
CMD_DEF(DIV, "/",   SECOND_PRIORITY, 
	fprintf(file_to_print, "\\fraq{");
	PrintLatexDiffNode(file_to_print, diff, node->left, node);
	fprintf(file_to_print, "}{");
	PrintLatexDiffNode(file_to_print, diff, node->right, node);
	fprintf(file_to_print, "}");
)
			 	    
CMD_DEF(ADD, "+",   THIRD_PRIORITY,  
	PRINT_IN_BINARY_OPER(+);
)
			 	    
CMD_DEF(SUB, "-",   THIRD_PRIORITY,  
	PRINT_IN_BINARY_OPER(-)
)
             
CMD_DEF(POW, "^",   FIRST_PRIORITY,
	PRINT_IN_BINARY_OPER(^)
)

CMD_DEF(COS, "cos", FIRST_PRIORITY,  ;)
             
CMD_DEF(SIN, "sin", FIRST_PRIORITY,  ;)
             
CMD_DEF(LN,  "ln",  FIRST_PRIORITY,  ;)

/*
* 
* HAPPY CODING!
*   ☺ <---(LET ME HUG YOU)
* --|--
*	|
*  / \
* 
*/