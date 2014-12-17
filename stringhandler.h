#ifndef STRINGHANDLER_H_INCLUDED
#define STRINGHANDLER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct StrSlice {
    char *str;
    struct StrSlice *next;
    struct StrSlice *endslice;
    unsigned int length;
} StrSlice;

/*
Create a StrSlice structure and initialize it
*/
StrSlice *CreateStrSlice(void);

/*
Get a substring char pointer.
@arguments
    string: a char pointer to the origin string
    position: the begin index
    length: substring length
*/
char *GetSubString(const char *string, int position, int length);

/*
Push a new string slice into a StrSlices structure.
@arguments
    ss: The pointer of a StrSlices structure
    str: The string to push back.
*/
void PushString(StrSlice *ss, const char *str);

/*
Delete a StrSlice structure.
@arguments
    ss: The StrSlice pointer to the target structure
*/
void DeleteStrSlice(StrSlice *ss);

/*
Delete useless spaces, \n, etc.
For example: "   \n{\"a\":1} \n" returns a new string "{\"a\":1}"
@arguments
    str: The target string
*/
char *DeleteSpaces(const char *str);

/*
Return a new StrSlices structure pointer, containing string slices of elements from a JSON array.
For example, "[3.14, "John Doe"]" will be return as following slices
    {"3.14", "\"John Doe\""}
@argument
    s: String of JSON array.
*/
StrSlice *GetArraySlices(const char* s);

/*
Return a new StrSlices structure pointer, containing string slices of elements from a JSON object.
For example, "{\"name\":\"John Doe\", \"Age\":26, \"hobbies\":[\"soccer\", \"PC Game\"]}" will be return as following slices
    {"\"name\"", "\"John Doe\"", "\"Age\"", "26", "[\"soccer\", \"PC Game\"]"}
    with each key with a value
@argument
    s: String of JSON array.
*/
StrSlice *GetObjectSlices(const char* s);

/*
Reformat the value string. Handling slash situations.
@arguments
    value: The target string
*/
char *FormatString(const char *value);

/*
Return a number format by a string
valid type: 1, 3.14, -4.5, 1.4e5, 1.4E-5, etc.
@arguments
    numstr: The target string
*/
double FormatNumber(char* numstr);

#ifdef __cplusplus
}
#endif


#endif // STRINGHANDLER_H_INCLUDED
