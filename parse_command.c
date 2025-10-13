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
    NullCommand.output_file =NULL;
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

struct Command *parse_command(char *line)
{
    int lastSpace = -1;
    int lastAmpersand = -1;
    int i = 0;
    struct Command *totalCommands = NULL;
    int commandsCount = 0;
    enum Finding findingValue = None;
    struct Command currentCommand;
    currentCommand.args = NULL;
    currentCommand.command = NULL;
    currentCommand.commandArgsString = NULL;
    currentCommand.output_file = NULL;
    int argCount = 0;
    char *commandArgsString = NULL;
    int start = 1;
    int redirectFound = 0;
    int afterRedirect = 0;
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
        case '>':
        {
            if (findingValue == DoubleQuote)
                continue;
    
            // Check for multiple redirects (error case)
            if (redirectFound) {
                // Multiple > symbols - we'll just use the last one for now
                // (The error will be caught when we see multiple filenames)
            }
            redirectFound = 1;
            afterRedirect = 1;
    
            // Parse any remaining argument before the >
            if (findingValue != Space && findingValue != None) {
                char *arg = arg_parse(line, i, lastSpace);
                if (arg != NULL) {
                    if (currentCommand.command == NULL) {
                        currentCommand.command = arg;
                    } else {
                        currentCommand.args = realloc(currentCommand.args, sizeof(char *) * (argCount + 2));
                        currentCommand.args[argCount++] = arg;
                        currentCommand.args[argCount] = NULL;
                    }
                }
            }
    
            lastSpace = i;
            findingValue = Space;
            break;
        }
        case '&':
        {
            if (findingValue == DoubleQuote)
                continue;
        runEnd:
            commandArgsString = arg_parse(line, i, lastAmpersand);
            currentCommand.commandArgsString = strdup(commandArgsString);
            free(commandArgsString);
            totalCommands = realloc(totalCommands, sizeof(struct Command) * (commandsCount + 2));
            totalCommands[commandsCount] = currentCommand;
            totalCommands[++commandsCount] = getNullCommand();
            currentCommand.args = NULL;
            currentCommand.command = NULL;
            currentCommand.commandArgsString = NULL;
            currentCommand.output_file = NULL;
            redirectFound = 0;
            afterRedirect = 0;
            argCount = 0;
            lastAmpersand = i;
            lastSpace = i;
            start =0;
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
            if (start) continue;
            if (findingValue == Space) {
                lastSpace = i;
                continue;
            }
            findingValue = Space;
            char *arg = arg_parse(line, i, lastSpace);
            if (arg == NULL)
                continue;
            
            if (afterRedirect){
                if(currentCommand.output_file != NULL){

                    free(arg);
                }else{
                    currentCommand.output_file = arg;
                }
                afterRedirect = 0;
            }else if(currentCommand.command == NULL){
                currentCommand.command = arg;
            }else{
                currentCommand.args = realloc(currentCommand.args, sizeof(char *) * (argCount + 2));
                currentCommand.args[argCount++] = arg;
                currentCommand.args[argCount] = NULL;
            }
            lastSpace = i;
            if (currentChar == '\n')
            {
                goto runEnd;
            }
            break;
        }
        default: {
            start =0;
            if (findingValue == Space) findingValue = None;
        }
        }
        if (currentChar == '\n' || currentChar == '\0')
            break;
        
    }
    return totalCommands;
}
void printCommand(struct Command command)
{
    printf("{\n");
    printf("  Command: %s\n", command.command);
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
    printf("  CommandArgsString: %s\n", command.commandArgsString);
    printf("}\n");
}
