#ifndef IO
#define IO


#include "poem.h"

void print_str(const char* str);

void record_poem(Poem* poem, const char* file_path);

void print_poem(Poem* poem, const char* name);

#endif