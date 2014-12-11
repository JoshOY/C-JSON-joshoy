#include "mylibs.h"

/*C substring function: It returns a pointer to the substring */
char *GetSubString(char *string, int position, int length)
{
   char *pointer;
   int c;

   pointer = malloc(length+1);

   if (pointer == NULL)
   {
      printf("Unable to allocate memory.\n");
      return NULL;
      //exit(EXIT_FAILURE);
   }

   for (c = 0 ; c < position -1 ; c++)
      string++;

   for (c = 0 ; c < length ; c++)
   {
      *(pointer+c) = *string;
      string++;
   }

   *(pointer+c) = '\0';

   return pointer;
}

StrSlices* GetArraySlices(const char* s)
{
    int length = strlen(s);
    unsigned int elementNum = 0, i = 0, lastIndex = 0;
    int quoteStatus = 0;
    //Here, inQuote is a boolean vector, marking up indexes of chars in quotes.
    //memset(inQuote, 0, length);
    for(i = 0; i < length; i++) {
        if(s[i] == '\"' && (i == 0 || s[i-1] != '\\')) {
            quoteStatus = (!quoteStatus);
        } else if((quoteStatus == 0) && (s[i] == ',')) {

        }
    }

    //free(inQuote);
}

char* HandleString(char *value)
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
