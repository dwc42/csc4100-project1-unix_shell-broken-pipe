#ifndef __PARSE_COMMAND_H
#define __PARSE_COMMAND_H
struct Command
{
    char *command;
    char **args;
    char *commandArgsString;
};
struct Command *parse_command(char *line);
void printCommand(struct Command command);
#endif