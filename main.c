#include "mylibs.h"
#include "JSON.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    /*
    printf("Hello world!\n");

    JSON* root = CreateObject();
    JSON* var_a = CreateNumber(3.14);
    JSON* var_s = CreateString("JoshOY");
    JSON* var_v = CreateArray();

    AddItemToObject(root, "a", var_a);
    AddItemToObject(root, "name", var_s);
    AddItemToObject(root, "info", var_v);
    AddItemToArray(var_v, CreateNumber(2333));
    AddItemToArray(var_v, CreateNumber(666));
    PrintJSON(root);
    DeleteJSON(root);
    */

    char *p = HandleString("\"Hello world!\n\"");
    printf("%s", p);
    return 0;
}
