#include "mylibs.h"

/*C substring function: It returns a pointer to the substring */
char *GetSubString(const char *str, int position, int length)
{
   char *string = str;
   char *pointer;
   int c;

   pointer = malloc(length+1);

   if (pointer == NULL)
   {
      printf("Unable to allocate memory.\n");
      return NULL;
      //exit(EXIT_FAILURE);
   }

   for (c = 0 ; c < position ; c++)
      string++;

   for (c = 0 ; c < length ; c++)
   {
      *(pointer+c) = *string;
      string++;
   }

   *(pointer+c) = '\0';

   return pointer;
}

void PushString(StrSlices *ss, const char *str)
{
    if(ss->len == 0) {
        ss->slice = (char **)malloc(sizeof(char *) * 1);
        ss->slice[0] = (char *)malloc(sizeof(char) * (strlen(str) + 1) );
        strcpy(ss->slice[0], str);
        ss->len += 1;
        ss->capacity = 1;
    } else if (ss->len == ss->capacity) {
        ss->slice = realloc(ss->slice, 2 * ss->capacity * sizeof(char **));
        memset(&(ss[ss->len]), NULL, ss->capacity);
        ss->capacity <<= 1;
        ss->slice[ss->len] = (char *)malloc(sizeof(char) * (strlen(str) + 1) );
        strcpy(ss->slice[ss->len], str);
        ss->len += 1;
    } else {
        ss->slice[ss->len] = (char *)malloc(sizeof(char) * (strlen(str) + 1) );
        strcpy(ss->slice[ss->len], str);
        ss->len += 1;
    }
    return;
}

void DeleteStrSlices(StrSlices *ss)
{
    int i;
    if(ss->len == 0) {
        free(ss);
    } else {
        for(i = 0; i < ss->len; i++) {
            free(ss->slice[i]);
        }
        free(ss->slice);
        free(ss);
    }
    return;
}

char *DeleteSpaces(const char* str)
{
    int frontcounter = 0;
    int backcounter = 0;
    int goOn = 1;
    while(goOn) {
        switch (str[frontcounter]) {
        case ' ':
        case '\n':
        case '\t':
            ++frontcounter;
            break;
        default:
            goOn = 0;
        }
    }
    goOn = 1;
    while(goOn) {
        switch (str[strlen(str) - 1 - backcounter]) {
        case ' ':
        case '\n':
        case '\t':
            ++backcounter;
            break;
        default:
            goOn = 0;
            break;
        }
    }
    if(!(frontcounter + backcounter)) {  // both == 0
        return GetSubString(str, 0, strlen(str));
    } else {
        char *p = GetSubString(str, frontcounter, strlen(str) - frontcounter - backcounter);
        return p;
    }
}


StrSlices *GetArraySlices(const char* s)
{
    StrSlices *rtn = (StrSlices *)malloc(sizeof(StrSlices));
    rtn->len = 0;
    rtn->capacity = 0;

    int length = strlen(s);
    unsigned int elementNum = 0, i = 1, lastIndex = 1;
    int quoteStatus = 0;
    char *newstr = NULL;

    for(i = 1; i < length - 1; i++) {
        if(s[i] == '\"' && (i == 0 || s[i-1] != '\\')) {
            quoteStatus = (!quoteStatus);
        } else if((quoteStatus == 0) && (s[i] == ',')) {
            newstr = GetSubString(s, lastIndex, i - lastIndex);
            lastIndex = i;
            PushString(rtn, newstr);
            free(newstr);
        }
    }
}

char* FormatString(char *value)
{
    //According to ECMA 404
    int index;
    int len = strlen(value);
    int realLength = 0;
    char *newstr = (char *)malloc((len - 1) * sizeof(char));
    memset(newstr, '\0', len);
    for(index = 1; index < len; index++) {
        switch(value[index]) {
        case '\\':
            index += 1;
            switch(value[index]) {
            case '\\': newstr[realLength] = '\\'; break;
            case '\"': newstr[realLength] = '\"'; break;
            case '/':  newstr[realLength] = '/';  break;
            case 'b':  newstr[realLength] = '\b'; break;
            case 'f':  newstr[realLength] = '\f'; break;
            case 'n':  newstr[realLength] = '\n'; break;
            case 'r':  newstr[realLength] = '\r'; break;
            case 't':  newstr[realLength] = '\t'; break;
            //case 'u':  newstr[realLength] = '\u'; break;
            default: printf("Warning: Invalid char \\%c ignored.\n", value[index]);
            }
            realLength += 1;
            break;
        case '\"':
            break;
        default:
            newstr[realLength] = value[index];
            realLength += 1;
            break;
        }
    }
    char *rtn = (char *)malloc(realLength * sizeof(char));
    strcpy(rtn, newstr);
    free(newstr);
    return rtn;
}
