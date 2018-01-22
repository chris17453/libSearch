#include "stdafx.h"
#include <ctype.h>

void error_exit(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void *memory_allocate(size_t size)
{
    void *allocated_memory = malloc(size);
    if(allocated_memory == NULL)
        error_exit("malloc failed, exiting.");
    return allocated_memory;
}

void *memory_reallocate(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if(size != 0 && new_ptr == NULL)
        error_exit("realloc failed, exiting.");
    return new_ptr;
}

char *string_duplicate(const char *str)
{
    char *new_str = strdup(str);
    if(new_str == NULL)
        error_exit("strdup failed, exiting.");
    return new_str;
}

char *string_duplicate_and_caseadjust(const char *str)
{
    char *new_str = strdup(str);
    char *x = new_str;
    if(new_str == NULL)
        error_exit("strdup failed, exiting.");
    while(*x != '\0')
        *x++ = tolower(*x);
    return new_str;
}

char *string_reverse(char *dst, const char *src)
{
    int length = strlen(src);
    for(int j = 0,i = length - 1; i >= 0; i--)
        dst[j++] = src[i];
    dst[length] = 0;
    return dst;
}

char *string_in_place_reverse(char *src)
{
    int length = strlen(src);
    int stop = length / 2;
    char temp;
    for(int i = 0; i < stop; i++)
    {
        temp = src[i];
        src[i] = src[length - i - 1];
        src[length - i - 1] = temp;
    }
    return src;
}

FILE *open_file(const char *path, const char *mode)
{
    FILE *file_p = fopen(path, mode);
    if(file_p == NULL)
    {
        fprintf(stderr, "libSearch: Unable to open %s\n", path);
        error_exit("Opening file failed, exiting.");
    }
    return file_p;
}

void readline(char *s, int size, FILE *stream)
{
    char *ret = fgets(s, size, stream);
    if(ret == NULL)
    {
        if(ferror(stream) != 0)
            error_exit("Reading line failed, exiting.");
        else
            memset(s, 0, size);
    }
}

void close_file(FILE *file_p)
{
    if(fclose(file_p) != 0)
        error_exit("Closing file failed, exiting.");
}


char * strtok_r(char *s1, const char *s2, char **lasts)
{
    char *ret;

    if (s1 == NULL)
        s1 = *lasts;
    while(*s1 && strchr(s2, *s1))
        ++s1;
    if(*s1 == '\0')
        return NULL;
    ret = s1;
    while(*s1 && !strchr(s2, *s1))
        ++s1;
    if(*s1)
        *s1++ = '\0';
    *lasts = s1;
    return ret;
}
