#include <string.h>
#include <malloc.h>
#include "JSON.h"

/****************
*     Create JSON    *
*****************/

JSON *createJSON(void)
{
    JSON *json = (JSON*)malloc(sizeof(JSON));
    json->valueint = -1;
    json->valuestring = NULL;
    json->valuedouble = NULL;
    json->valuearray = NULL;
    json->key = NULL;
    json->arraylength = 0;

    return json;
}

JSON *CreateNULL(void)
{
    JSON* rtn = createJSON();
    rtn->type = JSON_NULL;

    return rtn;
}

JSON *CreateTrue(void)
{
    JSON* rtn = createJSON();
    rtn->type = JSON_TRUE;
    rtn->valueint = 1;

    return rtn;
}

JSON *CreateFalse(void)
{
    JSON* rtn = createJSON();
    rtn->type = JSON_FALSE;
    rtn->valueint = 0;

    return rtn;
}

JSON *CreateBool(int b)
{
    JSON* rtn = NULL;
    if (b == 0) {
        rtn = CreateFalse();
    } else {
        rtn = CreateTrue();
    }
    return rtn;
}

JSON *CreateNumber(double num)
{
    JSON* rtn = createJSON();
    rtn->type = JSON_NUMBER;
    rtn->valuedouble = num;
    return rtn;
}

JSON *CreateString(const char* str)
{
    JSON* rtn = createJSON();
    rtn->type = JSON_STRING;
    rtn->valuestring = (char*)malloc(strlen(str));
    strcpy(rtn->valuestring, str);

    return rtn;
}
