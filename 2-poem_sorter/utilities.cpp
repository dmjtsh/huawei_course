#include <stdio.h>
#include <sys\stat.h>
#include <assert.h>

size_t get_file_size(FILE* file)
{
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    assert(file_size != -1);
    return file_size;
}