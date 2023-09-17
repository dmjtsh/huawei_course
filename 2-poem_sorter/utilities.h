#ifndef UTILITIES
#define UTILITIES

#include <stdio.h>

int get_file_size(const char* file_name);

char* get_file_text(const char* file_name);

int count_text_strings(const char* text);

size_t borders_rand(size_t left, size_t right);

#endif