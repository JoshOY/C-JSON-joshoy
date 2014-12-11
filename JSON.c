#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include "JSON.h"
#include "mylibs.h"

/*******************************
* Parse & Print functions
********************************/

JSON *ParseJSON(const char *value)
{
    int len = strlen(value);
    switch(value[0]) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0':
        //To number
        break;
    case '\"':
    case '\'':
        // To string
        break;
    case '{':
        if(value[len - 1] == '}') {
            //TODO, to object
        } else {
            printf("Exception: Invalid Syntax.");
            return NULL;
        }
        break;
    case '[':
         if(value[len - 1] == ']') {
            //TODO, to array
        } else {
            printf("Exception: Invalid Syntax.");
            return NULL;
        }
    }

}

void PrintJSON(JSON* item)
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
            printf("\"%s\":", item->keys[i]);
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
    json->keys = NULL;

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
    rtn->valuestring = strdup(str);
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
    rtn->child = (JSON **)malloc(1 * sizeof(JSON *));
    rtn->keys = (char **)malloc(1 * sizeof(char *));
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
        array->child = (JSON **)realloc(array->child, 2 * array->childcapacity * sizeof(JSON*));
        array->childcapacity <<= 1;
    }
    array->child[array->childlength] = item;
    array->childlength += 1;
    return;
}

void AddItemToObject(JSON* object, const char *key, JSON *value)
{
    if(GetItemInObject(object, key) != NULL) {
        printf("Exception: The key is already exist in the object.\n");
        return;
    }

    if(object->childlength == object->childcapacity) {
        object->child = (JSON **)realloc(object->child, 2 * object->childcapacity * sizeof(JSON*));
        object->keys = (char **)realloc(object->keys, 2 * object->childcapacity * sizeof(char *));
        int i;
        for(i = object->childlength + 1; i < object->childcapacity * 2; i++) {
            object->child[i] = NULL;
            object->keys[i] = NULL;
        }
        object->childcapacity <<= 1;
    }
    object->keys[object->childlength] = (char *)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(object->keys[object->childlength], key);
    object->child[object->childlength] = value;
    object->childlength += 1;
    return;
}

/*******************************
* Update functions
********************************/
void ReplaceItemInArray(JSON *array, int which, JSON *new_value)
{
    if(array->childlength <= which) {
        printf("Exception: Index out range.\n");
        return;
    }
    JSON* tmp = array->child[which];
    array->child[which] = new_value;
    DeleteJSON(tmp);
    return;
}

void ReplaceItemInObject(JSON *object, const char *key, JSON *new_value)
{
    int i, index;
    for(i = 0; i < object->childlength; i++) {
        if(strcmp(object->keys[i], key) == 0) {
            index = i;  //When found
            break;
        }
        if(i == object->childlength - 1) {
            printf("Exception: key not found.\n");
            return;
        }
    }
    JSON* tmp = object->child[index];
    object->child[index] = new_value;
    DeleteJSON(tmp);
    return;
}

/*******************************
* Remove/Delete functions
********************************/
JSON *DetachItemFromArray(JSON *array, int which)
{
    if(array->type != JSON_ARRAY) {
        printf("Exception: JSON type error.");
        return NULL;
    }
    if((which < 0) || (which >= array->childlength)) {
        return NULL;
    }
    JSON* tmp = array->child[which];
    int i;
    for(i = which; i < array->childlength; i++) {
        array->child[i] = array->child[i+1];
    }
    array->child[array->childlength - 1] = NULL;
    array->childlength -= 1;
    return tmp;
}

void DeleteItemFromArray(JSON *array, int which)
{
    DeleteJSON(DetachItemFromArray(array, which));
    return;
}

JSON *DetachItemFromObject(JSON *object, const char* key)
{
    if(object->type != JSON_OBJECT) {
        printf("Exception: JSON type error.");
        return ;
    }
    int index;
    for(index = 0; index < object->childlength; index++) {
        if(strcmp(object->keys[index], key) == 0) {
            break;
            if(index == object->childlength - 1) {
                return NULL;
            }
        }
    }
    JSON *tmp = object->child[index];
    int i;
    for(i = index; i < object->childlength; i++) {
        free(object->keys[i]);
        object->keys[i] = strdup(object->keys[i+1]);
        object->child[i] = object->child[i+1];
    }
    free(object->keys[object->childlength - 1]);
    object->keys[object->childlength - 1] = NULL;
    object->child[object->childlength - 1] = NULL;
    object->childlength -= 1;
    return tmp;
}

void DeleteItemFromObject(JSON *object, const char* key)
{
    DeleteJSON(DetachItemFromObject(object, key));
    return;
}

void DeleteJSON(JSON* json)
{
        if(json == NULL) {
                return;
        }
        if(json->type == JSON_STRING) {
                free(json->valuestring);
        }
        if(json->keys != NULL) {
                free(json->keys);
                json->keys = NULL;
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

/*******************************
* Read functions
********************************/

JSON *GetItemInArray(JSON *array, int which)
{
    if((which >= array->childlength) || (which < 0)) {
        return NULL;
    } else {
        return array->child[which];
    }
}

JSON *GetItemInObject(JSON *object, const char *key)
{
    int index;
    for(index = 0; index < object->childlength; index++) {
        if(strcmp(object->keys[index], key) == 0) {
            break;
            if(index == object->childlength - 1) {
                return NULL;
            }
        }
    }
    return object->child[index];
}

JSON* GetItemInJSON(JSON *json, const char *path)
{
    //TODO
    JSON *ptr = json;
    if(strcmp("/", path) == 0) {
        return ptr;
    }
    return NULL;
}
