#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

# ifdef _WIN32
#include <windows.h>
# endif

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "list.h"
}

void error_exit(const char *msg);
void *memory_allocate(size_t size);
void *memory_reallocate(void *ptr, size_t size);
char *string_duplicate(const char *str);
char *string_duplicate_and_caseadjust(const char *str);
char *string_reverse(char *dst, const char *src);
char *string_in_place_reverse(char *src);
FILE *open_file(const char *path, const char *mode);
void readline(char *s, int size, FILE *stream);
void close_file(FILE *file_p);

