#ifndef ELEMS_TYPE_H
#define ELEMS_TYPE_H

typedef double Elem_t;

#define ELEM_T_CPU_SSCANF(string, command, command_arg)      sscanf(string, "%d %lf", command, command_arg)
#define ELEM_T_ASM_SSCANF(string, command, command_arg)      sscanf(string, "%s %lf", command, command_arg)

#define ASM_CMD_SSCANF(string, command)                                                                                                                    \
	size_t length_of_cmd = 0;																															   \
	size_t length_of_cmd_w_arg = 0;																														   \
	COMMAND.arguments_num = sscanf(source_command_str, "%s%n %s%n",  COMMAND.ASM_cmd_code, &length_of_cmd, COMMAND.ASM_cmd_arg, &length_of_cmd_w_arg) - 1; \
	COMMAND.asm_cmd_len = length_of_cmd;																											       \
	if (COMMAND.arguments_num == 0)																														   \
		length_of_cmd_w_arg = length_of_cmd;																											   \
	else if(COMMAND.arguments_num > 0)																													   \
		COMMAND.asm_cmd_arg_len = length_of_cmd_w_arg - length_of_cmd - 1;																				   \
																																						   \
	for (size_t i = length_of_cmd_w_arg; source_command_str[i] != '\0'; i++)                                                                               \
	{																																					   \
		if(source_command_str[i] == ';')																												   \
		{																																				   \
			source_command_str[i] = '\0';																												   \
			break;																																		   \
		}																																				   \
		if(source_command_str[i] != 32 && source_command_str[i] != '\0')																				   \
		{																																				   \
			SetErrorBit((unsigned*)&COMMAND.error, TOO_MANY_ARGS);																						   \
			break;																																		   \
		}																																				   \
	}																																                       \


#define CMD_ARG_FWRITE(command_arg_ptr, file)                fwrite(command_arg_ptr, sizeof(double), 1, file);
#define CMD_W_ARG_FPRINTF(file, command, command_arg)        fprintf(file, "%d %lf", command, command_arg);
#define CMD_NO_ARG_FPRINTF(file, command)                    fprintf(file, "%d", command);

#define ELEM_T_SCANF(num)                                    scanf("%lf",  num)
#define ELEM_T_PRINTF(num)                                   printf("%lf", num)

#endif