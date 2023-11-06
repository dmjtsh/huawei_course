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
	{
	Elem_t num_to_push = {};
	if (command->cmd_arg_type & NUMBER_TYPE)
		num_to_push = command->CPU_cmd_arg;
	else if (command->cmd_arg_type & REGISTER_TYPE)
		num_to_push = GetReg(cpu, command->CPU_cmd_arg);

	if (command->cmd_arg_type & MEMORY_TYPE)
		StackPush(&cpu->stack, cpu->RAM[(size_t)num_to_push]);
	else
		StackPush(&cpu->stack, num_to_push);
	}
)

#define DO_OPER_WITH_TWO_NUMS(cpu, oper)               \
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

#define DO_OPER_WITH_ONE_NUM(cpu, oper)	               \
if (cpu->stack.size >= 1)							   \
{													   \
	Elem_t num = {};								   \
	StackPop(&cpu->stack, &num);					   \
	StackPush(&cpu->stack, oper(num));				   \
}													   \
else												   \
	SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE) \

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
	if (cpu->stack.size >= 1)
	{
		Elem_t num_to_pop = {};
		Elem_t arg = command->CPU_cmd_arg;
		CommandArgType arg_type = command->cmd_arg_type;

		StackPop(&cpu->stack, &num_to_pop);
		
		if (arg_type & MEMORY_TYPE && arg_type & NUMBER_TYPE)
			cpu->RAM[(size_t)arg] = num_to_pop;
		else if (arg_type & MEMORY_TYPE && arg_type & REGISTER_TYPE)
			cpu->RAM[(size_t)GetReg(cpu, arg)] = num_to_pop;
		else if (arg_type & REGISTER_TYPE)
			SetReg(cpu, arg, num_to_pop);
	}
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);
)
															      
#define JUMP()                                                     \
	{														       \
	Elem_t num_to_jump = {};			     			           \
	if (command->cmd_arg_type & NUMBER_TYPE)			           \
		num_to_jump = command->CPU_cmd_arg;				           \
	else if (command->cmd_arg_type & REGISTER_TYPE)		           \
		num_to_jump = GetReg(cpu, command->CPU_cmd_arg);           \
														           \
	if (command->cmd_arg_type & MEMORY_TYPE)			           \
		cpu->current_line_num = cpu->RAM[(size_t)num_to_jump - 1]; \
	else												           \
		cpu->current_line_num = num_to_jump - 1;                   \
	}

#define JUMP_ON_COND(cond)        \
	{							  \
	Elem_t num1 = {};             \
	Elem_t num2 = {};	          \
	StackPop(&cpu->stack, &num1); \
	StackPop(&cpu->stack, &num2); \
	if(num1 cond num2)            \
		JUMP()                    \
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
	StackPush(&cpu->stack, cpu->current_line_num);
	JUMP()
)

CMD_DEF(RET, 20, 0,
	if (cpu->stack.size >= 1)
	{
		Elem_t ret_address = {};
		StackPop(&cpu->stack, &ret_address);
		cpu->current_line_num = (size_t)ret_address;
	}
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);
)

CMD_DEF(OUTC, 21, 0,
	if (cpu->stack.size >= 1)
	{
		StackPop(&cpu->stack, &num_to_output);
		printf("%c", (char)num_to_output);
	}
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);)

CMD_DEF(DRAW, 22, 0,
	for(size_t i = 0; i < 10; i++)
	{
		for(size_t j = 0; j < 10; j++)
		{
			size_t current_index = i * 10 + j;
			if(cpu->RAM[current_index] != 0)
				printf("* ");
			else
				printf("- ");
		}
		printf("\n");
	}
)

#undef JUMP_ON_COND

CMD_DEF(HLT, -1, 0,
	CPUDtor(cpu);
	exit(228);
)