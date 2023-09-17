#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <time.h>


int get_file_size(const char* file_name)
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


char* get_file_text(const char* file_name)
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

int count_text_strings(const char* text)
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

size_t borders_rand(size_t left, size_t right)
{
    static bool is_seed_set = 0;
    if (!is_seed_set)
    {
        time_t t;
        srand((unsigned)time(&t));
    }
    return (left + rand() % (right - left));;
}