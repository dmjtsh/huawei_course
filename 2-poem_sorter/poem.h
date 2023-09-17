#ifndef POEM
#define POEM

struct Poem
{
	size_t poem_size;
	size_t strings_num;

	char* poem_text;
	char** strings;
};

void initialize_poem(Poem* poem, const char* file_name);

void record_poem(Poem* poem, const char* file_path);

void destroy_poem(Poem* poem);

#endif