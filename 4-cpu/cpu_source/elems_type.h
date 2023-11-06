#ifndef ELEMS_TYPE_H
#define ELEMS_TYPE_H

typedef double Elem_t;
#define ELEM_T_IDENTIFIER "%lf"

#define CPU_CMD_ELEM_T_SSCANF(string, command, command_arg) sscanf(string, "%d %lf", command, command_arg)

#define CMD_ARG_FWRITE(command_arg_ptr, file)               fwrite(command_arg_ptr, sizeof(double), 1, file);
#define CMD_W_ARG_FPRINTF(file, command, command_arg)       fprintf(file, "%d %lf", command, command_arg);
#define CMD_NO_ARG_FPRINTF(file, command)                   fprintf(file, "%d", command);

#define ELEM_T_SCANF(num)                                   scanf("%lf",  num)
#define ELEM_T_PRINTF(num)                                  printf("%lf", num)

#endif