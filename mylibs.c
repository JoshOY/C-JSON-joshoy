#include "mylibs.h"

#pragma warning(disable: 4996)
#define IS_ARRAY (0)
#define IS_OBEJCT (1)

/*C substring function: It returns a pointer to the substring */
char *GetSubString(const char *str, int position, int length)
{
	char *p = str + position;
	char *rtn = (char *)malloc(sizeof(char) * (length + 1));
	int i = 0;
	for (i = 0; i < length; i++) {
		rtn[i] = p[i];
	}
	rtn[length] = '\0';
	return rtn;
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
        ss->slice = (char **)realloc(ss->slice, 2 * ss->capacity * sizeof(char *));
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
		for (i = 0; i < ss->len; i++) {
			free(ss->slice[i]);
		}
		free(ss->slice);
		ss->slice = NULL;
    }
    return;
}

char *DeleteSpaces(const char *str)
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
    if(frontcounter == strlen(str)) {
        char *p = (char *)malloc(sizeof(char) * 1);
        p[0] = '\0';
        return p;
    }
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

StrSlices *GetArraySlices(const char *s)
{
    StrSlices *rtn = (StrSlices *)malloc(sizeof(StrSlices));
    rtn->len = 0;
    rtn->capacity = 0;
    rtn->type = IS_ARRAY;

    int length = strlen(s);
    unsigned int elementNum = 0, i = 0, lastIndex = 1;
    int quoteStatus = 0;
    char *sslice = NULL;

	while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t'){
		i++;
	}
	if (s[i] != '[') {
		printf("Exception: Invalid Array!");
		return NULL;
	}
	i += 1;
	lastIndex = i;
    for(; i < length; i++) {
        if((!quoteStatus) && (s[i] == '[' || s[i] == '{')) {
            char type = s[i];
            int layerCounter = 1;
			lastIndex = i;
            while(layerCounter) {
                ++i;
                if(s[i] == '[' || s[i] == '{') {
                    layerCounter += 1;
                } else if(s[i] == '}' || s[i] == ']') {
                    layerCounter -= 1;
                    if(layerCounter == 0) {
                        sslice = GetSubString(s, lastIndex, i - lastIndex + 1);
                        PushString(rtn, sslice);
                        free(sslice);
                        lastIndex = i + 1;
                    }
                }
            }
            continue;
        }

        if(s[i] == '\"'  && s[i-1] != '\\') {
            quoteStatus = (!quoteStatus);
        } else if ((quoteStatus == 0) && (s[i] == ',' || s[i] == ']')) {
            sslice = GetSubString(s, lastIndex, i - lastIndex);
            lastIndex = i+1;
            char *formatted = DeleteSpaces(sslice);
            if(strlen(formatted) != 0)
                PushString(rtn, formatted);
            //free(sslice);
            //free(formatted);
			sslice = NULL;
			formatted = NULL;
        }
    }

    return rtn;
}

StrSlices *GetObjectSlices(const char *s)
{
    StrSlices *rtn = (StrSlices *)malloc(sizeof(StrSlices));
    rtn->len = 0;
    rtn->capacity = 0;
    rtn->type = IS_OBEJCT;

    int length = strlen(s);
    unsigned int elementNum = 0, i = 0, lastIndex;
    int quoteStatus = 0;
    char *sslice = NULL;
	
	while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t'){
		i++;
	}
	if (s[i] != '{') {
		printf("Exception: Invalid object!");
		return NULL;
	}
	i += 1;
	lastIndex = i;
    for(; i < length; i++) {
        if((!quoteStatus) && (rtn->len & 0x1) && (s[i] == '[' || s[i] == '{')) {
            char type = s[i];
            int layerCounter = 1;
			lastIndex = i;
            while(layerCounter) {
                ++i;
                if(s[i] == '[' || s[i] == '{') {
                    layerCounter += 1;
                } else if(s[i] == '}' || s[i] == ']') {
                    layerCounter -= 1;
                    if(layerCounter == 0) {
                        sslice = GetSubString(s, lastIndex, i - lastIndex + 1);
                        PushString(rtn, sslice);
                        free(sslice);
                        lastIndex = i + 1;
                    }
                }
            }
            continue;
        }

        if(s[i] == '\"'  && s[i-1] != '\\') {
            quoteStatus = (!quoteStatus);
        } else if ((quoteStatus == 0) && (s[i] == ',' || s[i] == ':' || s[i] == '}')) {
            sslice = GetSubString(s, lastIndex, i - lastIndex);
            lastIndex = i+1;
            char *formatted = DeleteSpaces(sslice);
            if(strlen(formatted) != 0)
                PushString(rtn, formatted);
            //free(sslice);
			//free(formatted);
            sslice = NULL;
			formatted = NULL;
        }
    }

    return rtn;
}

char* FormatString(char *value)
{
    //According to ECMA 404
    int index;
    int len = strlen(value);
    int realLength = 0;
    char *newstr = (char *)malloc((len+1) * sizeof(char));
    memset(newstr, '?', len);
	newstr[len] = '\0';
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
    char *rtn = (char *)malloc((realLength + 1) * sizeof(char));
	int i;
	for (i = 0; i < realLength; ++i) {
		rtn[i] = newstr[i];
	}
	rtn[realLength] = '\0';
    free(newstr);
	newstr = NULL;
    return rtn;
}

