#ifndef __PARSE_COMMAND_H
#define __PARSE_COMMAND_H
struct Command
{
    char *command;
    char **args;
    char *commandArgsString;
    char *output_file;
};
struct Command *parse_command(char *line);
void printCommand(struct Command command);
void freeCommands(struct Command* commands);
#endif