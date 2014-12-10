/*
*	JSON.h
*	by JoshOY 2014 All rights reserved.
*/

#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

const int JSON_FALSE = 0;
const int JSON_TRUE = 1;
const int JSON_NULL = 2;
const int JSON_NUMBER = 3;
const int JSON_STRING = 4;
const int JSON_ARRAY = 5;
const int JSON_OBJECT = 6;

typedef int JSONType;

#ifdef __cplusplus
extern "C" {
#endif

struct json_object_iter
{
	char *key;
	struct json_object *val;
	struct lh_entry *entry;
};

/*******************************
* The cJSON structure
********************************/
typedef struct JSON {
	JSONType type;					/* The type of the item, as above. */
	char *valuestring;				/* The item's string, if type == JSON_STRING*/
	int valueint;					/* The item's number, if type == JSON_TRUE || JSON_FALSE */
	double valuedouble;				/* The item's number, if type == JSON_NUMBER */
	struct JSON **valuearray;    	/* The item's child nodes, if type == JSON_ARRAY || JSON_OBJECT */
	unsigned int arraylength;       /* array or object length */
} JSON;

/*******************************
* Parse & Print functions
********************************/

/*
解析JSON字符串
@arguments
	value: 需要解析为JSON的字符串
*/
JSON* ParseJSON(const char* value);

/*
解析JSON文件
@arguments
	file_name: 需要解析的文件名，函数返回指向解析后的JSON的指针
*/
JSON* ParseJSONFromFile(const char* file_name);

/*
将 JSON 无格式输出到屏幕
@arguments
    item: item 指向需要输出的 JSON
*/
void PrintJSON(JSON* item);

/*
Print JSON to a file with format
@arguments
    Item: JSON to output
    file_name: File name.
*/
void PrintJSONToFile(JSON *item, const char *file_name);

/*******************************
* Create JSON functions
********************************/

/*
Create a null JSON
*/
JSON* CreateNULL(void);

/*
Create a False JSON Object
*/
JSON* CreateFalse(void);

/*
Create a True JSON
*/
JSON* CreateTrue(void);

/*
Create a boolean JSON
@arguments
    b: 1 or 0, true or false
*/
JSON* CreateBool(int b);

/*
Create a number JSON
*/
JSON *CreateNumber(double num);

/*
Create a string JSON
*/
JSON *CreateString(const char *str);

/*
Create an array JSON
*/
JSON *CreateArray(void);

/*
Create an object
*/
JSON *CreateObject(void);


/*******************************
* Append functions
********************************/

/*
Append a JSON into an array
@arguments
	array: the JSON array to append
	item: the JSON to add
*/
void AddItemToArray(JSON *array, JSON *item);

/*
Add an item to an JSON object
@arguments
	object: the JSON object container
	key: a string, as index
	value: JSON for value
*/
void AddItemToObject(JSON *object, const char *key, JSON *value);


/*******************************
* Update functions
********************************/

/*
Replace an item in JSON array.
@arguments
	array: the array container
	which: index of the item to replace
	new_item: the new item
*/
void ReplaceItemInArray(JSON *array, int which, JSON *new_value);

#ifdef __cplusplus
}
#endif


#endif // JSON_H_INCLUDED
