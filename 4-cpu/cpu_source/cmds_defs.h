#define SPUSH(elem)  StackPush(&cpu->stack, elem)
#define SPOP(num)    StackPop(&cpu->stack, &num)
#define SIZE         cpu->stack.size

#define RAM cpu->RAM
#define CMD_TYPE     command->cmd_arg_type 
#define CMD_ARG      command->CPU_cmd_with_arg.arg
#define CURRENT_LINE cpu->current_line_num          

#define SET_ERROR(error)  SetErrorBit(&cpu->errors, error)

CMD_DEF(IN, 1, 0,
	printf("This is IN! Input number KSHCHTVR! ");
	correct_inputs_num = ELEM_T_SCANF(&input_num);
	if (correct_inputs_num == 1)
		SPUSH(input_num);
	else
		SET_ERROR(CPU_WRONG_INPUT);
)

CMD_DEF(OUT, 2, 0,
	if (SIZE >= 1)
	{
		SPOP(num_to_output);
		ELEM_T_PRINTF(num_to_output);
		printf("\n");
	}
	else
		SET_ERROR(CPU_WRONG_COMMAND_USAGE);
)

CMD_DEF(PUSH, 3, 1,
	SPUSH(GetProperArgument(cpu));
)

#define DO_OPER_WITH_TWO_NUMS(cpu, oper)               \
if (SIZE >= 2)                                         \
{				                                       \
	Elem_t num1 = {};                                  \
	Elem_t num2 = {};                                  \
	SPOP(num1);			                               \
	SPOP(num2);                                        \
	SPUSH(num1 oper num2);                             \
}                                                      \
else                                                   \
	SET_ERROR(CPU_WRONG_COMMAND_USAGE);                \


CMD_DEF(DIV, 4, 0,
	DO_OPER_WITH_TWO_NUMS(cpu, /);
)

CMD_DEF(ADD, 5, 0,
	DO_OPER_WITH_TWO_NUMS(cpu, +);
)

CMD_DEF(SUB, 6, 0,
	DO_OPER_WITH_TWO_NUMS(cpu, -);
)

CMD_DEF(MUL, 7, 0,
	DO_OPER_WITH_TWO_NUMS(cpu, *);
)

#undef DO_OPER_WITH_TWO_NUMS

#define DO_OPER_WITH_ONE_NUM(cpu, oper)	\
if (SIZE >= 1)							\
{										\
	Elem_t num = {};					\
	SPOP(num);					        \
	SPUSH(oper(num));				    \
}										\
else									\
	SET_ERROR(CPU_WRONG_COMMAND_USAGE)  \

CMD_DEF(SIN, 8, 0,
	DO_OPER_WITH_ONE_NUM(cpu, sin);
)

CMD_DEF(COS, 9, 0,
	DO_OPER_WITH_ONE_NUM(cpu, cos);
)

CMD_DEF(SQRT, 10, 0,
	DO_OPER_WITH_ONE_NUM(cpu, sqrt)
)
#undef DO_OPER_WITH_ONE_NUM

CMD_DEF(POP, 11, 1,
	if (SIZE >= 1)
	{
		Elem_t num_to_pop = {};

		SPOP(num_to_pop);
		
		if (CMD_TYPE & MEMORY_TYPE && CMD_TYPE & NUMBER_TYPE)
			RAM[(size_t)CMD_ARG] = num_to_pop;
		else if (CMD_TYPE & MEMORY_TYPE && CMD_TYPE & REGISTER_TYPE)
			RAM[(size_t)GetReg(cpu, CMD_ARG)] = num_to_pop;
		else if (CMD_TYPE & REGISTER_TYPE)
			SetReg(cpu, CMD_ARG, num_to_pop);
		else 
			assert(0);
	}
	else
		SET_ERROR(CPU_WRONG_COMMAND_USAGE);
)
															      
#define JUMP() CURRENT_LINE = GetProperArgument(cpu) - 1;

#define JUMP_ON_COND(cond) \
	{					   \
	Elem_t num1 = {};      \
	Elem_t num2 = {};	   \
	SPOP(num1);			   \
	SPOP(num2);			   \
	if(num1 cond num2)     \
		JUMP()             \
	}

CMD_DEF(JMP, 12, 1,
	JUMP()
)

CMD_DEF(JA, 13, 1,
	JUMP_ON_COND(>)
)

CMD_DEF(JB, 14, 1, 
	JUMP_ON_COND(<)
)

CMD_DEF(JAE, 15, 1,
	JUMP_ON_COND(>=)
)

CMD_DEF(JBE, 16, 1,
	JUMP_ON_COND(<=)
)

CMD_DEF(JE, 17, 1,
	JUMP_ON_COND(==)
)

CMD_DEF(JNE, 18, 1,
	JUMP_ON_COND(!=)
)

CMD_DEF(CALL, 19, 1,
	SPUSH(CURRENT_LINE);
	JUMP()
)

CMD_DEF(RET, 20, 0,
	if (SIZE >= 1)
	{
		Elem_t ret_address = {};
		SPOP(ret_address);
		CURRENT_LINE = (size_t)ret_address;
	}
	else
		SET_ERROR(CPU_WRONG_COMMAND_USAGE);
)

CMD_DEF(OUTC, 21, 0,
	if (SIZE >= 1)
	{
		SPOP(num_to_output);
		printf("%c", (char)num_to_output);
	}
	else
		SET_ERROR(CPU_WRONG_COMMAND_USAGE);
)

CMD_DEF(DRAW, 22, 0,
	DrawRAM(RAM);
)

#undef JUMP_ON_COND

CMD_DEF(HLT, -1, 0,
	CPUDtor(cpu);
	exit(228);
)

#undef SPUSH  
#undef SPOP    
#undef SIZE         

#undef RAM
#undef CMD_TYPE         
#undef CMD_ARG      
#undef CURRENT_LINE        

#undef SET_ERROR
