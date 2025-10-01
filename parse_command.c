#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libraries/string_to_int.h"


char *arg_parse(char *line, int i, int lastSpace)
{
    int start = lastSpace + 1;
    int len = i - start;
    if (len >= 1)
    {
        char *command = malloc(len + 1);
        strncpy(command, line + start, len);
        command[len] = '\0';
        return command;
    }
    return NULL;
}
enum Finding
{
    None = 0,
    Space = 1,
    DoubleQuote = 2,
    Ampersand = 3,
    AngleBracket = 4,
};

char **parse_command(char *line)
{
    struct Node *currentLinkedList = NULL;
    int lastSpace = -1;
    int i = 0;
    char *argscommand = NULL;
    enum Finding findingValue = None;
    struct Node *currentNode = NULL;
    char **args = NULL;
    int argCount = 0;
    for (;; i++)
    {
        char currentChar = line[i];
        switch (currentChar)
        {
        case '"':
        {
            if (findingValue == None)
            {
                lastSpace = i;
                findingValue = DoubleQuote;
            }
            else
            {
                char *arg = arg_parse(line, i, lastSpace);
                if (arg == NULL)
                    continue;
                args = realloc(args, sizeof(char *) * (argCount + 2));
                args[argCount++] = arg; // copy token
                args[argCount] = NULL;
                findingValue = None;
                lastSpace = i + 1;
            }
            break;
        }
        case '\n':
        case ' ':
        {
            if (findingValue != None && findingValue != Space)
                continue;
            char *arg = arg_parse(line, i, lastSpace);
            if (arg == NULL)
                continue;
            args = realloc(args, sizeof(char *) * (argCount + 2));
            args[argCount++] = arg; 
            args[argCount] = NULL;
            lastSpace = i;
        }
        }
        if (currentChar == '\0')
            break;
    }
    if (args == NULL)
    {
        args = malloc(sizeof(char *));
        argCount = 0;
    }
    return args;
}
