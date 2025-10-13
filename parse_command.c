#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse_command.h"
#include "libraries/string_int.h"

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
    NullCommand.output_file = NULL;
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
    // outpout stuct array of all the commands between the ampersands which will run in parrel
    struct Command *totalCommands = NULL;
    int commandsCount = 0;
    enum Finding findingValue = None;
    // current command the system is building
    struct Command currentCommand;
    // command intailsation
    currentCommand.args = NULL;
    currentCommand.command = NULL;
    currentCommand.commandArgsString = NULL;
    currentCommand.output_file = NULL;
    int argCount = 0;
    char *commandArgsString = NULL;
    // removes beginning spaces
    int start = 1;
    // control for when > was found for redirection
    int redirectFound = 0;
    int afterRedirect = 0;

    char currentChar = '\0';
    // for ingoring muliple &
    char lastChar = currentChar;
    do
    {
        int j = i++;
        lastChar = currentChar;
        currentChar = line[j];

        // printf("char: %d, %c - %d - %d - f:%d\n", currentChar, currentChar, j, lastSpace, findingValue);
        // printCommand(currentCommand);
        switch (currentChar)
        {
        case ' ':
        case '\t':

            break;
        default:
        {
            if (findingValue == Space)
                findingValue = None;
            break;
        }
        }

        switch (currentChar)
        {
        case '"':
        {

            if (findingValue != DoubleQuote)
            {
                lastSpace = j;
                findingValue = DoubleQuote;
            }
            else
            {
                char *arg = arg_parse(line, j, lastSpace);
                if (arg == NULL)
                {
                    findingValue = Space;
                    lastSpace = j;
                }
                if (afterRedirect)
                {
                    if (currentCommand.output_file != NULL)
                    {

                        free(arg);
                    }
                    else
                    {
                        currentCommand.output_file = arg;
                    }
                    afterRedirect = 0;
                }
                else
                {
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
                }
                findingValue = Space;
                lastSpace = j;
            }
            break;
        }
        case '>':
        {
            if (findingValue == DoubleQuote)
                continue;

            // Check for multiple redirects (error case)
            if (redirectFound)
            {
                // Multiple > symbols - we'll just use the last one for now
                // (The error will be caught when we see multiple filenames)
                continue;
            }
            redirectFound = 1;
            afterRedirect = 1;

            // Parse any remaining argument before the >
            if (findingValue != Space && findingValue != None)
            {
                char *arg = arg_parse(line, j, lastSpace);
                if (arg != NULL)
                {
                    if (currentCommand.command == NULL)
                    {
                        currentCommand.command = arg;
                    }
                    else
                    {
                        currentCommand.args = realloc(currentCommand.args, sizeof(char *) * (argCount + 2));
                        currentCommand.args[argCount++] = arg;
                        currentCommand.args[argCount] = NULL;
                    }
                }
            }

            lastSpace = j;
            findingValue = Space;
            break;
        }
        case '&':
        {
            if (lastChar == '&')
            {
                lastAmpersand = j; // keep separator window at the latest &
                lastSpace = j;     // prevent building a token like "&&&"
                continue;
            }
            if (findingValue == DoubleQuote)
                continue;
        runEnd:
            // Only append if there is a real command
            if (currentCommand.command != NULL)
            {
                // Build commandArgsString now (only when appending)
                commandArgsString = arg_parse(line, j, lastAmpersand);
                if (commandArgsString != NULL)
                {
                    currentCommand.commandArgsString = strdup(commandArgsString);
                    free(commandArgsString);
                }
                else
                {
                    currentCommand.commandArgsString = NULL;
                }

                totalCommands = realloc(totalCommands, sizeof(struct Command) * (commandsCount + 2));
                totalCommands[commandsCount] = currentCommand;
                commandsCount++;
                totalCommands[commandsCount] = getNullCommand(); // sentinel
            }
            else
            {
                // Drop empty/invalid command: free partial allocations
                if (currentCommand.args)
                {
                    for (int k = 0; currentCommand.args[k] != NULL; k++)
                        free(currentCommand.args[k]);
                    free(currentCommand.args);
                }
                free(currentCommand.command);
                free(currentCommand.commandArgsString);
                free(currentCommand.output_file);
            }

            // Reset for next command
            currentCommand.args = NULL;
            currentCommand.command = NULL;
            currentCommand.commandArgsString = NULL;
            currentCommand.output_file = NULL;
            redirectFound = 0;
            afterRedirect = 0;
            argCount = 0;
            lastAmpersand = j;
            lastSpace = j;
            start = 0;
            break;
        }
        case '\0':
        case '\n':
        case '\t':
        case ' ':
        {
            // so other findings ingore spaces
            if (findingValue == DoubleQuote) break;

            // Leading whitespace
            if (start) { lastSpace = j; break; }
            // ingores multiple spaces
            if ((currentChar == ' ' || currentChar == '\t') && findingValue == Space)
            {
                lastSpace = j;
                continue;
            }
            // gets the last string to add to arg or set command
            char *arg = arg_parse(line, j, lastSpace);
            // printf("arg parsed\n");

            if (arg == NULL)
            {
                if (currentChar == '\n' || currentChar == '\0')
                {
                    goto runEnd;
                }
                // printf("null\n");
                lastSpace = j;
                continue;
            }
            // printf("%s\n", arg);
            // handles text after > for redirection
            if (afterRedirect)
            {
                if (currentCommand.output_file != NULL)
                {

                    free(arg);
                }
                else
                {
                    currentCommand.output_file = arg;
                }
                afterRedirect = 0;
            }
            // sets command
            else if (currentCommand.command == NULL)
            {
                currentCommand.command = arg;
            }
            // pushs a arg
            else
            {
                currentCommand.args = realloc(currentCommand.args, sizeof(char *) * (argCount + 2));
                currentCommand.args[argCount++] = arg;
                currentCommand.args[argCount] = NULL;
            }
            // sets the last space for arg handling
            lastSpace = j;
            findingValue = Space;
            // runs end
            if (currentChar == '\n' || currentChar == '\0')
            {
                goto runEnd;
            }
            break;
        }
        default:
        {
            // if not space reset start
            start = 0;
            // if also not space reset space skiping
            if (findingValue == Space)
                findingValue = None;
            break;
        }
        }
    } while (currentChar != '\n' && currentChar != '\0');
    // return null pointer if null.
    if (totalCommands == NULL)
        return NULL;
    return totalCommands;
}
void printCommand(struct Command command)
{
    printf("{\n");
    printf("  Command: %s\n", command.command ? command.command : "NULL");
    printf("  Args: [");
    if (command.args)
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
    printf("  CommandArgsString: %s\n", command.commandArgsString ? command.commandArgsString : "NULL");
    printf("}\n");
}

void freeCommands(struct Command *commands)
{
    if (commands == NULL)
        return;
    for (int i = 0; commands[i].command != NULL; i++)
    {
        if (commands[i].args != NULL)
        {
            for (int j = 0; commands[i].args[j] != NULL; j++)
            {
                free(commands[i].args[j]);
            }
            free(commands[i].args);
        }
        free(commands[i].command);
        free(commands[i].commandArgsString);
        free(commands[i].output_file);
    }
    free(commands);
}
