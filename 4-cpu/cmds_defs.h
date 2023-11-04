CMD_DEF(IN, 1, 0,
	printf("This is IN! Input number KSHCHTVR! ");
	correct_inputs_num = ELEM_T_SCANF(&input_num);
	if (correct_inputs_num == 1)
		StackPush(&cpu->stack, input_num);
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_INPUT);
)

CMD_DEF(OUT, 2, 0,
	if (cpu->stack.size >= 1)
	{
		StackPop(&cpu->stack, &num_to_output);
		ELEM_T_PRINTF(num_to_output);
		printf("\n");
	}
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);
)

CMD_DEF(PUSH, 3, 1,
	switch((size_t)CURRENT_COMMAND.cmd_arg_type)
	{
	case NUMBER_TYPE:
		StackPush(&cpu->stack, CURRENT_COMMAND.CPU_cmd_arg);
		break;
	case REGISTER_TYPE:
		StackPush(&cpu->stack, GetReg(cpu, CURRENT_COMMAND.CPU_cmd_arg));
		break;
	case MEMORY_NUM_TYPE:
		StackPush(&cpu->stack, cpu->RAM[(size_t)CURRENT_COMMAND.CPU_cmd_arg]);
		break;
	case MEMORY_REG_TYPE:
		StackPush(&cpu->stack, cpu->RAM[(size_t)GetReg(cpu, CURRENT_COMMAND.CPU_cmd_arg)]);
		break;
	default:
		CURRENT_COMMAND.error = INVALID_SYNTAX;
	}
)

#define DO_MATH_OPER_WITH_TWO_STK_NUMS(cpu, oper)      \
if (cpu->stack.size >= 2)                              \
{				                                       \
	Elem_t num1 = {};                                  \
	Elem_t num2 = {};                                  \
	StackPop(&cpu->stack, &num1);			           \
	StackPop(&cpu->stack, &num2);                      \
	StackPush(&cpu->stack, num1 oper num2);            \
}                                                      \
else                                                   \
	SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE) \


CMD_DEF(DIV, 4, 0,
	DO_MATH_OPER_WITH_TWO_STK_NUMS(cpu, /);
)

CMD_DEF(ADD, 5, 0,
	DO_MATH_OPER_WITH_TWO_STK_NUMS(cpu, +);
)

CMD_DEF(SUB, 6, 0,
	DO_MATH_OPER_WITH_TWO_STK_NUMS(cpu, -);
)

CMD_DEF(MUL, 7, 0,
	DO_MATH_OPER_WITH_TWO_STK_NUMS(cpu, *);
)

#undef DO_MATH_OPER_WITH_TWO_STK_NUMS

#define DO_MATH_OPER_WITH_ONE_STK_NUM(cpu, oper)	   \
if (cpu->stack.size >= 1)							   \
{													   \
	Elem_t num = {};								   \
	StackPop(&cpu->stack, &num);					   \
	StackPush(&cpu->stack, oper(num));				   \
}													   \
else												   \
	SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE) \

CMD_DEF(SIN, 8, 0,
	DO_MATH_OPER_WITH_ONE_STK_NUM(cpu, sin);
)

CMD_DEF(COS, 9, 0,
	DO_MATH_OPER_WITH_ONE_STK_NUM(cpu, cos);
)

#undef DO_MATH_OPER_WITH_ONE_STK_NUM

CMD_DEF(POP, 10, 1,
	if (cpu->stack.size >= 1)
	{
		Elem_t num = {};
		StackPop(&cpu->stack, &num);
		switch(CURRENT_COMMAND.cmd_arg_type)
		{
		case REGISTER_TYPE:
			SetReg(cpu, CURRENT_COMMAND.CPU_cmd_arg, num);
			break;
		case MEMORY_NUM_TYPE:
			cpu->RAM[(size_t)CURRENT_COMMAND.CPU_cmd_arg] = num;
			break;
		case MEMORY_REG_TYPE:
			cpu->RAM[(size_t)GetReg(cpu, CURRENT_COMMAND.CPU_cmd_arg)] = num;
			break;
		default:
			CURRENT_COMMAND.error = INVALID_SYNTAX;
		}
	}
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);
)

#define JUMP()																					   \
	switch(CURRENT_COMMAND.cmd_arg_type)														   \
	{																							   \
		case NUMBER_TYPE:																		   \
			cpu->num_of_current_line = CURRENT_COMMAND.CPU_cmd_arg;								   \
			break;																		           \
		case REGISTER_TYPE:																           \
			cpu->num_of_current_line = GetReg(cpu, CURRENT_COMMAND.CPU_cmd_arg);				   \
			break;																		           \
		case MEMORY_NUM_TYPE:															           \
			cpu->num_of_current_line = cpu->RAM[(size_t)CURRENT_COMMAND.CPU_cmd_arg];			   \
			break;																		           \
		case MEMORY_REG_TYPE:															           \
			cpu->num_of_current_line = cpu->RAM[(size_t)GetReg(cpu, CURRENT_COMMAND.CPU_cmd_arg)]; \
			break;																                   \
		default:																                   \
			CURRENT_COMMAND.error = INVALID_SYNTAX;										           \
	}

#define JUMP_ON_COND(cond)        \
	{							  \
	Elem_t num1 = {};			  \
	Elem_t num2 = {};			  \
	StackPop(&cpu->stack, &num1); \
	StackPop(&cpu->stack, &num2); \
	if(num1 cond num2)            \
		JUMP()                    \
	}

CMD_DEF(JMP, 11, 1,
	JUMP()
)

CMD_DEF(JA, 12, 1,
	JUMP_ON_COND(>)
)

CMD_DEF(JB, 13, 1, 
	JUMP_ON_COND(<)
)

CMD_DEF(JAE, 14, 1,
	JUMP_ON_COND(>=)
)

CMD_DEF(JBE, 15, 1,
	JUMP_ON_COND(<=)
)

CMD_DEF(JE, 16, 1,
	JUMP_ON_COND(==)
)

CMD_DEF(JNE, 17, 1,
	JUMP_ON_COND(!=)
)

CMD_DEF(CALL, 18, 1,
	StackPush(&cpu->func_stack, cpu->num_of_current_line);
	JUMP()
)

CMD_DEF(RET, 19, 0,
	if (cpu->func_stack.size >= 1)
	{
		Elem_t ret_address = {};
		StackPop(&cpu->func_stack, &ret_address);
		cpu->num_of_current_line = (size_t)ret_address;
	}
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);
)

#undef JUMP_ON_COND

CMD_DEF(HLT, -1, 0,
	CPUDtor(cpu);
	exit(228);
)