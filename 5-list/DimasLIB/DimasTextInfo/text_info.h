#ifndef TEXT_INFO_H
#define TEXT_INFO_H

struct TextString
{
	char* str;
	int len;
};

struct TextInfo
{
	size_t text_size;
	size_t strings_num;

	bool is_okay;
	char* text;
	TextString* text_strings;
};

const int TEXT_INFO_ERROR = -1;

int TextInfoCtor(TextInfo* text_info, const char* file_name);

int TextInfoDtor(TextInfo* text_info);

#endif // TEXT_INFO_H