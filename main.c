#include "stringhandler.h"
#include "JSON.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int   ch;
    int   index;
    JSON *json = NULL;
    FILE *fp   = NULL;

    opterr = 0;
    if (argc == 1) {
        printf("\n");
        printf("JSON parser by JoshOY, 2014 All rights reserved.\n");
        printf("Type C-JSON -h for more info.\n");
        printf("\n");
        return 0;
    }

    while ((ch = getopt(argc, argv, "hp:r:w:")) != -1) {
        switch (ch) {
            case 'h':
            printf("====================================================\n");
            printf("* JSON parser by JoshOY, 2014 All rights reserved. *\n");
            printf("====================================================\n");
            printf("\n");
            printf("Usage: C-JSON [-h] [-p <JSON string>] [-r <path>] [-w <path>] ");
            printf("\n");
            break;

            case 'p':
                if (json != NULL) {
                    DeleteJSON(json);
                }
                printf("Parsing \"%s\"...", optarg);
                json = ParseJSON(optarg);
                if (json != NULL) {
                    printf("\nParse JSON success.\n");
                    PrintJSON(json);
                }
                else {
                    exit(1);
                }
                break;

            case 'r':
                if (json != NULL) {
                    DeleteJSON(json);
                }
                json = ParseJSONFromFile(optarg);
                if (json != NULL) {
                    printf("\nRead from file success.\n");
                    PrintJSON(json);
                }
                else {
                	exit(1);
                }
                break;

            case 'w':
                if (json == NULL) {
                    json = CreateNULL();
                }
                PrintJSONToFile(json, optarg);
                printf("Successfully written to file \"%s\".\n", optarg);
                DeleteJSON(json);
                break;

            default:
                printf("Unknown command.\n");
        }
    }

    if (json != NULL) {
        DeleteJSON(json);
    }

    return 0;
}
