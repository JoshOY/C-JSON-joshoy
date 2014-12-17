#include "stringhandler.h"
#include "JSON.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    //DEBUG PART

    /*
    printf("Hello world!\n");

    char *p = FormatString("\t\"Hello world!\"\n");
    printf("===%s===\n", p);
    char *p2 = DeleteSpaces(p);
    printf("===%s===\n", p2);
    free(p);
    free(p2);
	StrSlices *ss = (StrSlices *)malloc(sizeof(StrSlices));
	free(ss);




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
    DeleteJSON(root);*/

    char str[] = "[  true,  \t\n false,\"Hello!\", \"World!\", \"JoshOY.\",   \t \n{\"a\"  : \"test\", \"b\":\"test2\"}]";
	char* s = (char *)malloc(sizeof(char) * (strlen(str) + 1) );
	strcpy(s, str);
	JSON* json = ParseJSON(s);
	PrintJSON(json);
	DeleteJSON(json);
	free(s);

    return 0;
}
