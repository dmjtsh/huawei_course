#ifndef ELEMS_TYPE_H
#define ELEMS_TYPE_H

typedef double Elem_t;

#define ELEM_T_CPU_SSCANF(string, command, command_arg) sscanf(string, "%d %lf", command, command_arg)
#define ELEM_T_ASM_SSCANF(string, command, command_arg) sscanf(string, "%s %lf", command, command_arg)
#define ELEM_T_SCANF(num)                               scanf("%lf", num);
#define ELEM_T_PRINTF(num)                              printf("%lf", num);

#endif