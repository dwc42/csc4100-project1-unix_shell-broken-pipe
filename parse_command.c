#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse_command.h"
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

struct Command getNullCommand()
{
    struct Command NullCommand;
    NullCommand.args = NULL;
    NullCommand.command = NULL;
    NullCommand.commandArgsString = NULL;
    return NullCommand;
}

enum Finding
{
    None = 0,
    Space = 1,
    DoubleQuote = 2,
    Ampersand = 3,
    AngleBracket = 4,
};

struct Command parse_command(char *line)
{
    int lastSpace = -1;
    int lastAmpersand = -1;
    int i = 0;
    // struct Command *totalCommands = NULL;
    int commandsCount = 0;
    enum Finding findingValue = None;
    struct Command currentCommand;
    currentCommand.args = NULL;
    currentCommand.command = NULL;
    currentCommand.commandArgsString = NULL;
    int argCount = 0;
    char *commandArgsString = NULL;
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
                if (currentCommand.command == NULL)
                {
                    currentCommand.command = arg;
                }
                else
                {
                    currentCommand.args = realloc(currentCommand.args, sizeof(char *) * (argCount + 2));
                    currentCommand.args[argCount++] = arg; // copy token
                    currentCommand.args[argCount] = NULL;
                }
                findingValue = None;
                lastSpace = i + 1;
            }
            break;
        }
        case '&':
        {
        runEnd:
            commandArgsString = arg_parse(line, i, lastAmpersand);
            currentCommand.commandArgsString = strdup(commandArgsString);
            free(commandArgsString);
            // totalCommands = realloc(totalCommands, sizeof(struct Command) * (commandsCount + 2));
            // totalCommands[commandsCount] = currentCommand
            // totalCommands[++commandsCount] = getNullCommand();
            // argCount = 0;
            // args = malloc(sizeof(NULL));
            // args = NULL;
            break;
        }
        case '\0':
        {
            goto runEnd;
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
            if (currentCommand.command == NULL)
            {
                currentCommand.command = arg;
            }
            else
            {
                currentCommand.args = realloc(currentCommand.args, sizeof(char *) * (argCount + 2));
                currentCommand.args[argCount++] = arg; // copy token
                currentCommand.args[argCount] = NULL;
            }
            lastSpace = i;
            if (currentChar == '\n')
            {
                printf("8933763763");
                goto runEnd;
            }
            break;
        }
        }
        if (currentChar == '\n' || currentChar == '\0')
            break;
    }
    return currentCommand;
}
void printCommand(struct Command command)
{
    printf("{\n");
    printf("  Command: %s,\n", command.command);
    printf("  Args: [");
    if (command.args != NULL)
        for (int i = 0; command.args[i] != NULL;)
        {
            printf("\"%s\"", command.args[i]);
            if (command.args[++i] != NULL)
            {
                printf(",");
            }
            else
            {
                break;
            }
        }
    printf("]\n");
    printf("  CommandArgsString: %s,\n", command.commandArgsString);
    printf("}\n");
}
