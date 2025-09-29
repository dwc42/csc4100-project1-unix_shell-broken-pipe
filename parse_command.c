#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libraries/string_to_int.h"

//github copolit wrote this as c make no sense

char **split(const char *str, const char *delim)
{
    char *s = strdup(str); // make a modifiable copy
    if (!s)
        return NULL;

    char **result = NULL;
    char *token;
    int count =0;
    while ((token = strsep(&s, delim)) != NULL)
    {
        // grow the array for each token
        result = realloc(result, sizeof(char *) * (count + 1));
        result[count] = strdup(token); // copy token
        count++;
    }
    result[count] = NULL;
    free(s); // strsep moves the pointer, so only the base copy would remain — no need to free
    return result;
}
char *arg_parse(char *line, int i, int lastSpace)
{
    int len = i - (lastSpace + 1);
    if (len > 1)
    {
        char *command = malloc(sizeof(char) * (len + 1));
        strncpy(command, line + (lastSpace + 1), len);
        command[len] = '\0';
        return command;
    }
}
enum Finding {
    None = 0,
    Space = 1,
    DoubleQuote = 2,
    Ampersand = 3,
    AngleBracket = 4,
};

char** parse_command(char *line)
{
    struct Node *currentLinkedList = NULL;
    int lastSpace = -1;
    int i = 0;
    char *command = NULL;
    enum Finding findingValue = None;
    struct Node *currentNode = NULL;

    for (; ; i++)
    {
        char currentChar = line[i];
        switch (currentChar)
        {
        case '"':
        {
            if (findingValue = None) {
                lastSpace = i;
                findingValue = DoubleQuote;
            } else {

            }
        }
        case '\n':
        case '\0':
        case ' ':
        {

            
        }
        }
        if (currentChar == '\0') break;
    }
    printf("command: %s\n", command);
    printf("args:\n");
    printList(currentLinkedList);
}
