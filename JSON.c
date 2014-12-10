#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include "JSON.h"

/*******************************
* Parse & Print functions
********************************/

PrintJSON(JSON* item)
{
    int i;
    switch(item->type) {
    case JSON_NULL:
        printf("null");
        break;
    case JSON_TRUE:
        printf("true");
        break;
    case JSON_FALSE:
        printf("false");
        break;
    case JSON_NUMBER:
        printf("%g", item->valuedouble);
        break;
    case JSON_STRING:
        printf("\"%s\"", item->valuestring);
        break;
    case JSON_ARRAY:
        printf("[");
        for(i = 0; i < item->childlength; i++) {
            PrintJSON(item->child[i]);
            if(i + 1 != item->childlength) {
                printf(",");
            }
        }
        printf("]");
        break;
    case JSON_OBJECT:
        printf("{");
        for(i = 0; i < item->childlength; i++) {
            printf("\"%s\":", item->child[i]->key);
            PrintJSON(item->child[i]);
            if(i + 1 != item->childlength) {
                printf(",");
            }
        }
        printf("}");
        break;
    }
}

/*******************************
* Create functions
********************************/
//
JSON *_CreateJSON(void)
{
    JSON *json = (JSON*)malloc(sizeof(JSON));
    json->valueint = -1;
    json->valuestring = NULL;
    json->valuedouble = 0;
    json->child = NULL;
    json->next = NULL;
    json->preview = NULL;
    json->childlength = 0;
    json->childcapacity = 0;
    json->index = 0;
    json->key = NULL;

    return json;
}

JSON *CreateNULL(void)
{
    JSON* rtn = _CreateJSON();
    rtn->type = JSON_NULL;

    return rtn;
}

JSON *CreateTrue(void)
{
    JSON* rtn = _CreateJSON();
    rtn->type = JSON_TRUE;
    rtn->valueint = 1;

    return rtn;
}

JSON *CreateFalse(void)
{
    JSON* rtn = _CreateJSON();
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
    JSON* rtn = _CreateJSON();
    rtn->type = JSON_NUMBER;
    rtn->valuedouble = num;
    return rtn;
}

JSON *CreateString(const char* str)
{
    JSON* rtn = _CreateJSON();
    rtn->type = JSON_STRING;
    rtn->valuestring = (char*)malloc(strlen(str) + 1);
    strcpy(rtn->valuestring, str);

    return rtn;
}

JSON *CreateArray(void)
{
        JSON* rtn = _CreateJSON();
        rtn->type = JSON_ARRAY;
        rtn->child = (JSON**)malloc(sizeof(JSON*));
        rtn->childlength = 0;
        rtn->childcapacity = 1;

        return rtn;
}

JSON *CreateObject(void)
{
        JSON* rtn = _CreateJSON();
        rtn->type = JSON_OBJECT;
        rtn->child = (JSON**)malloc(1 *sizeof(JSON*));
        rtn->childlength = 0;
        rtn->childcapacity = 1;

        return rtn;
}

/*******************************
* Append functions
********************************/

void AddItemToArray(JSON *array, JSON *item)
{
    if(array->childlength == array->childcapacity) {
        array->child = realloc(array->child, 2 * array->childcapacity * sizeof(JSON*));
        array->childcapacity <<= 1;
    }
    array->child[array->childlength] = item;
    array->childlength += 1;
    return;
}

void AddItemToObject(JSON* object, const char *key, JSON *value)
{
    if(object->childlength == object->childcapacity) {
        object->child = realloc(object->child, 2 * object->childcapacity * sizeof(JSON*));
        int i;
        for(i= object->childlength + 1; i < object->childcapacity * 2; i++) {
            object->child[i] = NULL;
        }
        object->childcapacity <<= 1;
    }
    object->child[object->childlength] = value;
    if(value->key == NULL) {
        value->key = (char*)malloc(strlen(key) + 1);
    }
    strcpy(value->key, key);
    object->childlength += 1;
    return;
}

/*******************************
* Remove/Delete functions
********************************/
void DeleteJSON(JSON* json)
{
        if(json == NULL) {
                return;
        }
        if(json->type == JSON_STRING) {
                free(json->valuestring);
        }
        if(json->key != NULL) {
                free(json->key);
                json->key = NULL;
        }
        if(json->childlength != 0) {
                unsigned int i;
                for(i = 0; i < json->childlength; i++) {
                        if(json->child[i] != NULL)
                            DeleteJSON(json->child[i]);
                }
        }
        if((json->type == JSON_OBJECT) || (json->type == JSON_ARRAY))
            free(json->child);
        free(json);
        return;
}
