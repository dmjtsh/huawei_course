struct Poem
{
	size_t poem_size;
	size_t strings_num;

	char* poem_text;
	char** strings;
};

void initialize_poem(Poem* poem, char* file_name);

void destroy_poem(Poem* poem);