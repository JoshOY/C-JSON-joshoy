#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "JSON.h"
#include "stringhandler.h"

/*******************************
* Parse & Print functions
********************************/

JSON *ParseJSON(const char *value)
{
    JSON       *rtn   = NULL;
    StrSlice   *ss    = NULL;
    StrSlice   *iter  = NULL;
    int         len   = strlen(value);
    int         index = 0;

    switch(value[0]) {
    case '-':
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
		//return CreateNumber(FormatNumber(value));
		return NULL;
        break;

    case '\"':
        // To string
        return CreateString(FormatString(value));
        break;

    case '{':
		rtn  =  CreateObject();
		ss   =  GetObjectSlices(value);
		iter =  ss;

        while (iter != NULL) {
            AddItemToObject(rtn, FormatString(iter->str), ParseJSON(DeleteSpaces(iter->next->str)));
            iter = iter->next->next;
        }
		return rtn;
        break;

    case '[':
		rtn  =  CreateArray();
		ss   =  GetArraySlices(value);
		iter =  ss;

        while (iter != NULL) {
            AddItemToArray(rtn, ParseJSON(DeleteSpaces(iter->str)));
            iter = iter->next;
        }
		return rtn;
		break;

    case 'n':
		if (strcmp(value, "null") == 0) {
			return CreateNULL();
		}
		else {
			printf("Exception: Invalid Syntax \"%s\"", value);
			return NULL;
		}

    case 't':
		if (strcmp(value, "true") == 0) {
			return CreateTrue();
		}
		else {
			printf("Exception: Invalid Syntax \"%s\"", value);
			return NULL;
		}
        break;

    case 'f':
		if (strcmp(value, "false") == 0) {
			return CreateFalse();
		}
		else {
			printf("Exception: Invalid Syntax \"%s\"", value);
			return NULL;
		}
        break;
    }
}

void PrintJSON(JSON* item)
{
    int i;
    JSON *iter;

    switch(item->type) {
    case JSON_NULL:   printf("null");                      break;
    case JSON_TRUE:   printf("true");                      break;
    case JSON_FALSE:  printf("false");                     break;
    case JSON_NUMBER: printf("%g", item->valuedouble);     break;
    case JSON_STRING: printf("\"%s\"", item->valuestring); break;

    case JSON_ARRAY:
        printf("[");
        iter = item->childstart;
        for (i = 0; i < item->childlength; ++i) {
            PrintJSON(iter);
            if (i != item->childlength - 1)
                printf(",");
            iter = iter->next;
        }
        printf("]");
        break;

    case JSON_OBJECT:
        printf("{");
        iter = item->childstart;
        for (i = 0; i < item->childlength; ++i) {
            printf("\"%s\":", iter->key);
            PrintJSON(iter);
            if (i != item->childlength - 1)
                printf(",");
            iter = iter->next;
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
    JSON *json         = (JSON*)malloc(sizeof(JSON));
    json->valueint     = -1;
    json->valuestring  = NULL;
    json->valuedouble  = 0;
    json->childstart   = NULL;
    json->childend     = NULL;
    json->next         = NULL;
    json->preview      = NULL;
    json->childlength  = 0;
    json->index        = 0;
    json->key          = NULL;

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
    }
    else {
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
    rtn->valuestring = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(rtn->valuestring, str);
    rtn->valuestring[strlen(str)] = '\0';
    return rtn;
}

JSON *CreateArray(void)
{
    JSON* rtn = _CreateJSON();
    rtn->type = JSON_ARRAY;

    return rtn;
}

JSON *CreateObject(void)
{
    JSON* rtn = _CreateJSON();
    rtn->type = JSON_OBJECT;

    return rtn;
}

/*******************************
* Append functions
********************************/

void AddItemToArray(JSON *array, JSON *item)
{
    array->childlength += 1;
    if(array->childend == NULL) {
        array->childstart = item;
        array->childend = item;
    } else {
        array->childend->next = item;
        array->childend->next->preview = array->childend;
        array->childend->next->next = NULL;
        array->childend = array->childend->next;
    }
    return;
}

void AddItemToObject(JSON* object, const char *key, JSON *item)
{
    object->childlength += 1;
    if(object->childend == NULL) {
        object->childstart = item;
        object->childend = item;
    } else {
        object->childend->next = item;
        object->childend->next->preview = object->childend;
        object->childend->next->next = NULL;
        object->childend = object->childend->next;
    }

    object->childend->key = (char *)malloc(sizeof(char) * (strlen(key) + 1) );
    strcpy(object->childend->key, key);
    return;
}

/*******************************
* Update functions
********************************/
void ReplaceItemInArray(JSON *array, int which, JSON *new_value)
{
    int   i    = 0;
    JSON *iter = array->childstart;
    JSON *prev = NULL;
    JSON *next = NULL;

    if (array->childlength <= which) {
        printf("Exception: Index out range.\n");
        return;
    }

    for (i = 0; i < which; i++) {
        iter = iter->next;
    }

    prev = iter->next;
    next = iter->preview;

    DeleteJSON(iter);

    prev->next = new_value;
    next->preview = new_value;
    new_value->next = next;
    new_value->preview = prev;

    return;
}

void ReplaceItemInObject(JSON *object, const char *key, JSON *new_value)
{
    JSON *iter = object->childstart;
    JSON *prev = NULL;
    JSON *next = NULL;

    while (iter != NULL) {
        if (strcmp(iter->key, key) == 0) {
            prev = iter->next;
            next = iter->preview;

            DeleteJSON(iter);

            prev->next = new_value;
            next->preview = new_value;
            new_value->next = next;
            new_value->preview = prev;
            new_value->key = (char *)malloc(sizeof(char) * (strlen(key) + 1) );
            strcpy(new_value->key, key);
            return;
        }
        iter = iter->next;
    }
    printf("Exception: key \"%s\" not found.", key);
    return;
}

/*******************************
* Remove/Delete functions
********************************/
JSON *DetachItemFromArray(JSON *array, int which)
{
    //Firstly we check the type of the array JSON.
    if (array->type != JSON_ARRAY) {
        printf("Exception: JSON type error.\n");
        return NULL;
    }
    //Check if the which var is out of range.
    if ((which < 0) || (which >= array->childlength)) {
        printf("Exception: JSON array out of range.\n");
        return NULL;
    }

    //Get the target array element
    JSON *iter = array->childstart;
    int   i    = 0;

    for (i = 0; i < which; ++i) {
        iter = iter->next;
    }

    //Adjust the array
    array->childstart->childlength -= 1;

    if (iter->preview != NULL)
        iter->preview->next = iter->next;
    else
        array->childstart = iter->next;

    if (iter->next != NULL)
        iter->next->preview = iter->preview;
    else
        iter->childend = iter->preview;

    iter->next = NULL;
    iter->preview = NULL;

    return iter;
}

void DeleteItemFromArray(JSON *array, int which)
{
    DeleteJSON(DetachItemFromArray(array, which));
    return;
}

JSON *DetachItemFromObject(JSON *object, const char* key)
{
    if (object->type != JSON_OBJECT) {
        printf("Exception: JSON type error.\n");
        return NULL;
    }

    JSON *iter = object->childstart;
    while (iter != NULL) {
        if (strcmp(iter->key, key) == 0) {
            break;
        }
        else {
            iter = iter->next;
        }
    }
    if (iter == NULL) {
        printf("Exception: key not found.\n");
        return NULL;
    }

    if (iter->preview != NULL)
        iter->preview->next = iter->next;
    else
        object->childstart = iter->next;

    if (iter->next != NULL)
        iter->next->preview = iter->preview;
    else
        iter->childend = iter->preview;

    iter->next = NULL;
    iter->preview = NULL;

    return iter;
}

void DeleteItemFromObject(JSON *object, const char* key)
{
    DeleteJSON(DetachItemFromObject(object, key));
    return;
}

void DeleteJSON(JSON* item)
{
    if (item == NULL)
        return;
    if (item->type = JSON_STRING)
        free(item->valuestring);
    if (item->key != NULL) {
        free(item->key);
        item->key = NULL;
    }
    if (item->childlength != 0) {
        JSON *iter = item->childstart;
        JSON *next_iter;
        while (iter != NULL) {
            next_iter = iter->next;
            DeleteJSON(iter);
            iter = next_iter;
        }
        item->childstart = NULL;
        item->childend = NULL;
    }
    free(item);
    return;
}

/*******************************
* Read functions
********************************/

JSON *GetItemInArray(JSON *array, int which)
{
    if (which < 0 || which >= array->childlength) {
        printf("Exception: index out of range.\n");
        return NULL;
    }

    int   counter = 0;
    JSON *iter    = array->childstart;

    for (counter = 0; counter < which; counter++) {
        iter = iter->next;
    }
    return iter;
}

JSON *GetItemInObject(JSON *object, const char *key)
{
    int   counter = 0;
    JSON *iter    = object->childstart;

    while (iter != NULL) {
        if (strcmp(iter->key, key) == 0) {
            break;
        }
        iter = iter->next;
    }
    if (iter == NULL) {
        printf("Exception: key not found in the object.\n");
    }
    return iter;
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
