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

/*******************************
* The cJSON structure
********************************/
typedef struct JSON {
    JSONType type;					/* The type of the item, as above. */
	char *valuestring;				/* The item's string, if type == JSON_STRING*/
	int valueint;					/* The item's number, if type == JSON_TRUE || JSON_FALSE */
    double valuedouble;				/* The item's number, if type == JSON_NUMBER */

    unsigned int index;                     /* The item's index, if type == JSON_ARRAY */
    char *key;                              /* The item's hey, if type == JSON_OBJECT */
    struct JSON *childstart;
    struct JSON *childend;
    struct JSON *next;
    struct JSON *preview;
    unsigned int childlength;
} JSON;

/*******************************
* Parse & Print functions
********************************/

/*
Create a new JSON NOT INITIALIZED.
DO NOT USE.
*/
JSON *_CreateJSON(void);

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
@arguments
	str: initialize with string str.
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

/*
Replace an item in JSON object.
@arguments
array: the object container
key: key of the item to replace
new_item: the new item
*/
void ReplaceItemInObject(JSON *object, const char *key, JSON *new_value);


/*******************************
* Remove/Delete functions
********************************/
/*
Pop and return an element from a JSON array, not deleting it.
@arguments
    array: The JSON array executing this function
    which: The index of element in array
*/
JSON *DetachItemFromArray(JSON *array, int which);


/*
Delete an item from a JSON array.
@arguments
	array: The JSON array executing this function
	which: The index of element in array
*/
void DeleteItemFromArray(JSON *array, int which);

/*
Pop and return an element from a JSON object, not deleting it.
@arguments
	object: The JSON object executing this function
	which: The key of element in object
*/
JSON *DetachItemFromObject(JSON *object, const char *key);

/*
Delete an element in a JSON object.
@arguments
	object: The JSON object executing this function
	which: The key of element in object
*/
void DeleteItemFromObject(JSON *object, const char *key);

/*
Delete a JSON, including its sub-elements.
@arguments
    item: the JSON pointer to free
*/
void DeleteJSON(JSON *item);


/*******************************
* Duplicate functions
********************************/

/*
Duplicate a JSON and return the new JSON pointer.
@arguments
	item: the JSON item to duplicate
	recurse: 1 or 0 (true or false), representing deepcopy or not
*/
JSON *Duplicate(JSON *item, int recurse);



/*******************************
* Read functions
********************************/

/*
Get an item in a JSON array. Return a pointer.
(Return NULL if not found.)
@arguments
	array: JSON array
	which: The index of item in the array
*/
JSON *GetItemInArray(JSON *array, int which);

/*
Get an item in a JSON object. Return a pointer.
(Return NULL if not found.)
@arguments
	object: JSON object
	key: The key of item in the object
*/
JSON *GetItemInObject(JSON *object, const char *key);

/*
Get the element in a JSON array or object with given relative path.
@arguments
	json: JSON array or object
	path: Relative path of the item.
*/
JSON *GetItemInJSON(JSON *json, const char* path);


#ifdef __cplusplus
}
#endif


#endif // JSON_H_INCLUDED
