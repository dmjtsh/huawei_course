#include <stdio.h>

extern FILE* LOGGER;

#define LOG(str, ...) fprintf(LOGGER, str, __VA_ARGS__)