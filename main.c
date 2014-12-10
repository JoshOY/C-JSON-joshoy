#include "JSON.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Hello world!\n");

    char *str = "Hi!I'm Josh.";
    char *p = (char*)malloc(strlen(str));

    strcpy(p, str);
    puts(p);
    free(p);
    return 0;
}
