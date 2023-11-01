CMD_DEF(IN, 0, 0,
	printf("This is IN! Input number KSHCHTVR! ");
	correct_inputs_num = ELEM_T_SCANF(&input_num);
	if (correct_inputs_num == 1)
		StackPush(&cpu->stack, input_num);
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_INPUT);
)

CMD_DEF(OUT, 1, 0,
	if (cpu->stack.size >= 1)
	{
		StackPop(&cpu->stack, &num_to_output);
		ELEM_T_PRINTF(num_to_output);
		printf("\n");
	}
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);
)

CMD_DEF(PUSH, 2, 1,
	if (command.cmd_arg_type == NUMBER_TYPE)
		StackPush(&cpu->stack, command.cmd_arg);
	else if(command.cmd_arg_type == REGISTER_TYPE)
	{
		if (IsDoubleNumsEqual(command.cmd_arg, (double)(int)command.cmd_arg))
			StackPush(&cpu->stack, GetReg(cpu, command.cmd_arg));
		else 
			SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);
	}
)

#define DoMathOperWithTwoStkNums(cpu, oper)            \
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


CMD_DEF(DIV, 3, 0,
	DoMathOperWithTwoStkNums(cpu, /);
)

CMD_DEF(ADD, 4, 0,
	DoMathOperWithTwoStkNums(cpu, +);
)

CMD_DEF(SUB, 5, 0,
	DoMathOperWithTwoStkNums(cpu, -);
)

CMD_DEF(MUL, 6, 0,
	DoMathOperWithTwoStkNums(cpu, *);
)

#undef DoMathOperWithTwoStkNums

#define DoMathOperWithOneStkNum(cpu, oper)				\
if (cpu->stack.size >= 1)								\
{														\
	Elem_t num = {};									\
	StackPop(&cpu->stack, &num);						\
	StackPush(&cpu->stack, oper(num));					\
}														\
else													\
	SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE)  \

CMD_DEF(SIN, 7, 0,
	DoMathOperWithOneStkNum(cpu, sin);
)

CMD_DEF(COS, 8, 0,
	DoMathOperWithOneStkNum(cpu, cos);
)

#undef DoMathOperWithOneStkNum

CMD_DEF(POP, 9, 1,
	if (cpu->stack.size >= 1 && IsDoubleNumsEqual(command.cmd_arg, (double)(int)command.cmd_arg))
	{
		Elem_t num = {};
		StackPop(&cpu->stack, &num);
		SetReg(cpu, command.cmd_arg, num);
	}
	else
		SetErrorBit(&cpu->errors, CPU_WRONG_COMMAND_USAGE);
)

CMD_DEF(HLT, -1, 0,
	CPUDtor(cpu);
	exit(228);
)