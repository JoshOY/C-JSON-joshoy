#include "stringhandler.h"
#define NULL (0)

char *GetSubString(const char *string, int position, int length)
{
    char *p = string + position;
	char *rtn = (char *)malloc(sizeof(char) * (length + 1));
	int i = 0;

	for (i = 0; i < length; i++) {
		rtn[i] = p[i];
	}

	rtn[length] = '\0';
	return rtn;
}

StrSlice *CreateStrSlice()
{
    StrSlice *rtn = (StrSlice *)malloc(sizeof(StrSlice));
    rtn->str      = NULL;
    rtn->next     = NULL;
    rtn->endslice = NULL;
    rtn->length   = 0;

    return rtn;
}

void PushString(StrSlice *ss, const char *str)
{
    if (ss->length == 0) {
        ss->str = (char *)malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(ss->str, str);
        ss->length = 1;
    }
    else if (ss->length == 1) {
        ss->next = CreateStrSlice();
        ss->endslice = ss->next;
        ss->next->str = (char *)malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(ss->next->str, str);
        ss->length = 2;
    }
    else {
        StrSlice *p = CreateStrSlice();
        ss->endslice->next = p;
        ss->endslice = p;
        p->str = (char *)malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(p->str, str);
        ss->length += 1;
    }
}

void DeleteStrSlice(StrSlice *ss)
{
    if(ss->str != NULL)
        free(ss->str);
    if(ss->next != NULL) {
        DeleteStrSlice(ss->next);
    }
    free(ss);
    return;
}

char *DeleteSpaces(const char *str)
{
    int frontcounter = 0;
    int backcounter  = 0;
    int goOn         = 1;

    while(goOn) {
        switch (str[frontcounter]) {
        case ' ' :
        case '\n':
        case '\t':
            ++frontcounter;
            break;
        default:
            goOn = 0;
        }
    }
    goOn = 1;
    if(frontcounter == strlen(str)) {
        char *p = (char *)malloc(sizeof(char) * 1);
        p[0] = '\0';
        return p;
    }
    while(goOn) {
        switch (str[strlen(str) - 1 - backcounter]) {
        case ' ' :
        case '\n':
        case '\t':
            ++backcounter;
            break;
        default:
            goOn = 0;
            break;
        }
    }
    if(frontcounter + backcounter == 0) {
        return GetSubString(str, 0, strlen(str));
    }
    else {
        return GetSubString(str, frontcounter, strlen(str) - frontcounter - backcounter);
    }
}

StrSlice *GetArraySlices(const char *s) {
    StrSlice     *rtn         = CreateStrSlice();
    int           s_len       = strlen(s);
    unsigned int  i           = 0;
    unsigned int  lastIndex   = 1;
    int           quoteStatus = 0;
    char         *sslice      = NULL;

    for (i = 1; i < s_len; ++i) {
        if ( (!quoteStatus) && (s[i] == '[' || s[i] == '{') ) {
            char type = s[i];
            int layerCounter = 1;
			lastIndex = i;
            while (layerCounter) {
                i += 1;
                if (s[i] == '[' || s[i] == '{') {
                    layerCounter += 1;
                }
                else if (s[i] == '}' || s[i] == ']') {
                    layerCounter -= 1;
                    if (layerCounter == 0) {
                        /*
                        if(type != s[i]) {
                            printf("Exception: Invalid syntax.\n");
                            DeleteStrSlice(rtn);
                            return NULL;
                        }
                        */
                        sslice = GetSubString(s, lastIndex, i - lastIndex + 1);
                        PushString(rtn, sslice);
                        free(sslice);
                        lastIndex = i + 1;
                    }
                }
            }
            continue;
        }

        if (s[i] == '\"'  && s[i-1] != '\\') {
            quoteStatus = (!quoteStatus);
        }
        else if ((quoteStatus == 0) && (s[i] == ',' || s[i] == ']')) {
            char *tmp = GetSubString(s, lastIndex, i - lastIndex);
            lastIndex = i+1;
            sslice    = DeleteSpaces(tmp);
            if (strlen(sslice) != 0)
                PushString(rtn, sslice);

            free(tmp);
            free(sslice);
			sslice = NULL;
        }
    }

    return rtn;
}

StrSlice *GetObjectSlices(const char *s)
{
    StrSlice     *rtn           = CreateStrSlice();
    int           s_len         = strlen(s);
    unsigned int  i             = 0;
    unsigned int  lastIndex     = 1;
    int           quoteStatus   = 0;
    char         *sslice        = NULL;
    int           on_key_status = 1;  // Now reading a key?

    for(i = 1; i < s_len; ++i) {

        if ((!on_key_status) && (!quoteStatus) && (s[i] == '[' || s[i] == '{')) {
            char type = s[i];
            int layerCounter = 1;
			lastIndex = i;
            while(layerCounter) {
                ++i;
                if (s[i] == '[' || s[i] == '{') {
                    layerCounter += 1;
                }
                else if (s[i] == '}' || s[i] == ']') {
                    layerCounter -= 1;
                    if(layerCounter == 0) {
                        /*
                        if(type != s[i]) {
                            printf("Exception: Invalid syntax.\n");
                            DeleteStrSlice(rtn);
                            return NULL;
                        }
                        */
                        sslice = GetSubString(s, lastIndex, i - lastIndex + 1);
                        PushString(rtn, sslice);
                        free(sslice);
                        lastIndex = i + 1;
                    }
                }
            }
            continue;
        }

        if (s[i] == '\"'  && s[i-1] != '\\') {
            quoteStatus = (!quoteStatus);
        }
        else if ((quoteStatus == 0) && (s[i] == ',' || s[i] == ':' || s[i] == '}')) {
            char *tmp = GetSubString(s, lastIndex, i - lastIndex);
            lastIndex = i+1;
            sslice    = DeleteSpaces(tmp);
            if (strlen(sslice) != 0)
                PushString(rtn, sslice);

            free(tmp);
            free(sslice);
            sslice = NULL;
            on_key_status = !on_key_status;
        }

    }

    return rtn;
}


char* FormatString(const char *value)
{
    //According to ECMA 404
    int   i          = 0;
    int   index      = 0;
    int   len        = strlen(value);
    int   realLength = 0;
    char *newstr     = (char *)malloc((len+1) * sizeof(char));
    char *rtn        = NULL;

    memset(newstr, '?', len);
	newstr[len]      = '\0';

    for (index = 1; index < len; index++) {
        switch (value[index]) {
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
            default:   printf("Warning: Invalid char \\%c ignored.\n", value[index]);
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

    rtn = (char *)malloc((realLength + 1) * sizeof(char));

	for (i = 0; i < realLength; ++i) {
		rtn[i] = newstr[i];
	}
	rtn[realLength] = '\0';
    free(newstr);
	newstr = NULL;
    return rtn;
}
