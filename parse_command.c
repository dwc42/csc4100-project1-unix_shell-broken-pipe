#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libraries/linked_list.h"
#include "libraries/string_to_int.h"
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
    char *lastArg = NULL;
    int findingValue = 0;
    struct Node *currentNode = NULL;
    for (; line[i] != '\0'; i++)
    {
        char currentChar = line[i];
        switch (currentChar)
        {
        case ' ':
        {
            char *arg = arg_parse(line, i, lastSpace);
            printf("%s", arg);
            if (command == NULL)
            {
                command = arg;
            }
            else
            {
                lastArg = arg;
            }
            if (findingValue)
            {
                char *endptr;
                int value = stringToInt(arg, endptr);
                if (*endptr == '\0')
                {
                    currentNode->intData = value;
                }
                else
                {
                    currentNode->stringData = arg;
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
                currentNode = createNode(arg, -1, NULL);
            }
            lastSpace = i;
        }
        }
    }
    char *arg = arg_parse(line, i, lastSpace);
    if (command == NULL)
    {
        command = arg;
    }
    else
    {
        lastArg = arg;
    }
    if (findingValue)
    {
        char *endptr;
        int value = stringToInt(arg, endptr);
        if (*endptr == '\0')
        {
            currentNode->intData = value;
        }
        else
        {
            currentNode->stringData = arg;
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
        currentNode = createNode(arg, -1, NULL);
    }
    printList(currentLinkedList);
}
