#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <time.h>


int get_file_size(char* file_name)
{
    if (!file_name)
        return -1;

    FILE* file = NULL;
    fopen_s(&file, file_name, "r");
    if (!file)
        return -1;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);
    return file_size;
}


char* get_file_text(char* file_name)
{
    if(!file_name)
        return NULL;
    int file_size = get_file_size(file_name);
    assert(file_size != -1);


    FILE* file = NULL;
    fopen_s(&file, file_name, "r");
    if (!file)
        return NULL;

    char* text = (char*)calloc(file_size, sizeof(char));
    if (!text)
        return NULL;
    fread(text, sizeof(char), file_size, file);

    fclose(file);
    return text;
}

int count_text_indents(char* text)
{
    if (!text)
        return -1;
    int text_size = strlen(text);

    size_t indents_num = 0;
    char* start_ch = text;
    char* current_ch = text;
    while (current_ch - start_ch <= text_size)
    {
        if (*current_ch == '\n')
            indents_num++;
        current_ch++;
    }

    return indents_num;
}

size_t borders_rand(size_t left, size_t right)
{
    time_t t;
    srand((unsigned)time(&t));
    return (left + rand() % (right - left));;
}