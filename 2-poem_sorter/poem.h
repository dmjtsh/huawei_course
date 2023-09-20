#ifndef POEM
#define POEM

struct PoemString
{
	char* str;
	int len;
};

struct Poem
{
	size_t poem_size;
	size_t strings_num;

	char* poem_text;
	PoemString* poem_strings;
};

void initialize_poem(Poem* poem, const char* file_name);

void destroy_poem(Poem* poem);

#endif