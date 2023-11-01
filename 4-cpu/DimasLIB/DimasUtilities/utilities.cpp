#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <ctype.h>

#include "utilities.h"

void SetErrorBit(unsigned* error, int error_bit)
{
    assert(error != NULL);
    *error |= error_bit;
}

void UnsetErrorBit(unsigned* error, int error_bit)
{
    assert(error != NULL);
    *error &= ~error_bit;
}

int GetFileSize(const char* file_name)
{
    if (!file_name)
        return -1;

    FILE* file = NULL;
    file = fopen(file_name, "r");
    if (!file)
        return -1;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);
    return file_size;
}

char* GetFileText(const char* file_name)
{
    if(!file_name)
        return NULL;
    int file_size = GetFileSize(file_name);
    assert(file_size != -1);

    FILE* file = NULL;
    file = fopen(file_name, "r");
    if (!file)
        return NULL;

    char* text = (char*)calloc(file_size, sizeof(char));
    if (!text)
        return NULL;
    
    size_t n_chars = fread(text, sizeof(char), file_size, file);

    fclose(file);
    return text;
}

int CountTextStrings(const char* text)
{
    if (!text)
        return -1;
    int text_size = strlen(text);

    size_t indents_num = 0;

    for (const char* current_p = text; current_p - text <= text_size; current_p++)
        if (*current_p == '\n')
            indents_num++;

    return indents_num;
}

size_t BordersRand(size_t left, size_t right)
{
    static bool is_seed_set = 0;
    if (!is_seed_set)
    {
        time_t t;
        srand((unsigned)time(&t));
        is_seed_set = 1;
    }
    return (left + rand() % (right - left));
}

bool IsDoubleNumsEqual(double num1, double num2)
{
    return fabs(num1 - num2) < EPSILON;
}

size_t StrToNum(const char* str, size_t str_len, bool* is_str_num)
{
	*is_str_num = true;
	size_t final_num = 0;
	for (size_t i = 0; i < str_len; i++)
	{
		if(!isdigit(str[i]))
		{
			*is_str_num = false;
			break;
		}
		final_num += (str[i] - '0') * pow(10, str_len-i-1);  
	}
	return final_num;
}