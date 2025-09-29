#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libraries/linked_list.h"
#include "libraries/string_to_int.h"

//github copolit wrote this as c make no sense
char *strcopy_safe(const char *src) {
    if (!src) return NULL;
    char *copy = malloc(strlen(src) + 1);
    if (copy) strcpy(copy, src);
    return copy;
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
int parse_command(char *line)
{
    struct Node *currentLinkedList = NULL;
    int lastSpace = -1;
    int i = 0;
    char *command = NULL;
    int findingValue = 0;
    struct Node *currentNode = NULL;
    for (; ; i++)
    {
        char currentChar = line[i];
        switch (currentChar)
        {
        case '\0':
        case ' ':
        {
            char *arg = arg_parse(line, i, lastSpace);
            //printf("%s\n", arg);
            if (command == NULL)
            {
                command = strcopy_safe(arg);
                goto space;
            }

            if (findingValue)
            {
                findingValue = 0;
                char *endptr;
                int value = stringToInt(arg, &endptr);
                printf("%d", value);
                printf("%d\n", *endptr);
                if (*endptr == '\0' || *endptr == '\n' || *endptr == ' ')
                {
                    currentNode->intData = value;
                }
                else
                {
                    currentNode->stringData = strcopy_safe(arg);
                }

                if (currentLinkedList == NULL)
                {
                    currentLinkedList = currentNode;
                }
                else
                {
                    insertAtBeginning(&currentLinkedList, currentNode);
                }
            }
            else
            {
                findingValue = 1;
                currentNode = createNode(strcopy_safe(arg), -1, NULL);
            }
            space:
            lastSpace = i;
        }
        }
        if (currentChar == '\0') break;
    }
    printf("command: %s\n", command);
    printf("args:\n");
    printList(currentLinkedList);
}
