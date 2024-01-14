#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <string.h>
#include <cstdlib>

#include "text_info.h"
#include "../DimasUtilities/utilities.h"

void InitializeTextStrings(TextInfo* text_info)
{
	assert(text_info->text != NULL);
	assert(text_info->strings_num != -1);
	assert(text_info->text_size != -1);

	text_info->text_strings = (TextString*)calloc(text_info->strings_num+1, sizeof(TextString));

	assert(text_info->text_strings != NULL);

	text_info->text_strings[0].str = text_info->text;

	size_t strings_counter = 0;

	char* start_p = text_info->text;
	for (char* current_p = start_p; current_p - start_p <= text_info->text_size; current_p++)
	{
		text_info->text_strings[strings_counter].len++;
		
		if (*current_p == '\n')
		{
			*current_p = '\0';
			text_info->text_strings[strings_counter].len--;
			text_info->text_strings[++strings_counter].str = current_p + 1;
			assert(text_info->text_strings[strings_counter].str != NULL);
		}

	}
}

int TextInfoCtor(TextInfo* text_info, const char* file_name)
{
	if (file_name == NULL || text_info == NULL)
		return TEXT_INFO_ERROR;

	strcpy(text_info->file_name, file_name); 

	text_info->is_okay = 1;
	text_info->text_size = GetFileSize(file_name);
	
	text_info->text = GetFileText(file_name);
	if (!text_info->text)
	{
		text_info->is_okay = 0;
		return TEXT_INFO_ERROR;
	}
	
	text_info->strings_num = CountTextStrings(text_info->text);
	if (text_info->strings_num == -1)
	{
		text_info->is_okay = 0;
		return TEXT_INFO_ERROR;
	}
	InitializeTextStrings(text_info);
	
	return 0;
}

int TextInfoDtor(TextInfo* text_info)
{
	if (!text_info || text_info->text_strings || !text_info->text)
		return TEXT_INFO_ERROR;

	free(text_info->text_strings);
	free(text_info->text);

	return 0;
}