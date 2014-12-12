#include "mylibs.h"
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


    char *p = FormatString("\t\"Hello world!\"\n");
    printf("===%s===\n", p);
    char *p2 = DeleteSpaces(p);
    printf("===%s===\n", p2);
    free(p);
    free(p2);
	StrSlices *ss = (StrSlices *)malloc(sizeof(StrSlices));
	free(ss);*/

	char *s = "  {  \"a\":2,   \"b\":\n3,\"c\" :[4,5,6 ],\n \"d\"  : {\"age\":20} }\n ";
    StrSlices *ss = GetObjectSlices(s);
    int i;
    for(i = 0; i < ss->len; i++) {
        printf("%s\n", ss->slice[i]);
    }
    printf("Length: %d\n", ss->len);
    DeleteStrSlices(ss);
	
    return 0;
}
