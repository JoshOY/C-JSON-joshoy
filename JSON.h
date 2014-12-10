/*
*	JSON.h
*	by JoshOY 2014 All rights reserved.
*/

#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

#define JSON_FALSE (0)
#define JSON_TRUE (1)
#define JSON_NULL (2)
#define JSON_NUMBER (3)
#define JSON_STRING (4)
#define JSON_ARRAY (5)
#define JSON_OBJECT (6)

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

    struct JSON **child;
    unsigned int index;                     /* The item's index, if type == JSON_ARRAY */
    char *key;                                      /* The item's key, if type == JSON_OBJECT */
    struct JSON *next;
    struct JSON *preview;
    unsigned int childlength;
    unsigned int childcapacity;
} JSON;

/*******************************
* Parse & Print functions
********************************/

/*
Parse JSON string
@arguments
	value: JSON string to parse
*/
JSON* ParseJSON(const char* value);

/*
Parse a JSON from a file
@arguments
	file_name: Name of a JSON file.
*/
JSON* ParseJSONFromFile(const char* file_name);

/*
Print JSON to terminal without format
@arguments
    item: The JSON to output
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

/*******************************
* Remove/Delete functions
********************************/
/*
Pop an element from a JSON array, not deleting it.
@arguments
    array: The JSON array executing this function
    which: The index of element in array
*/
JSON *DetachItemFromArray(JSON *array, int which);


/*

*/

/*
Delete a JSON, including its sub-elements.
@arguments
    item: the JSON pointer to free
*/
void DeleteJSON(JSON* item);


#ifdef __cplusplus
}
#endif


#endif // JSON_H_INCLUDED
