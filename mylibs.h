#ifndef MYLIBS_H_INCLUDED
#define MYLIBS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct StrSlices {
    char **slice;
    unsigned int len;
} StrSlices;

char *GetSubString(char *string, int position, int length);
StrSlices* GetArraySlices(const char* s);
char* HandleString(char *value);

#endif // MYLIBS_H_INCLUDED
