#include "error_table.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "libraries/string_int.h"
char *getErrorMessage(enum Errors errorId)
{
    switch (errorId)
    {
    case CommandParseFailure:
    {
        return "An error has occurred"; // "Command Parse Failure Error has occured";
    }
    case EXEVPFailure:
    {
        return "An error has occurred"; // "Execuable Run Failure Error has occured";
    }
    case FileOpenFailure:
    {
        return "An error has occurred"; // "File Open Failure Error has occured";
    }
    case ForkFailure:
    {
        return "An error has occurred"; // "Fork Failure Error has occured";
    }
    case PathInvalid:
    {
        return "An error has occurred"; // "Path Invaild Error has occured";
    }
    case TooManyArgs:
    {
        return "An error has occurred"; // "Too Many Arguments Error has occured";
    }
    case TooFewArgs:
    {
        return "An error has occurred"; // "Too Few Arguments Error has occured";
    }
    case ValueIsNull:
    {
        return "An error has occurred"; // "Value is Null Error has occured";
    }
    case Unknown:
    default:
    {
        return "An error has occurred"; // "Unknown Error has occured";
    }
    }
};

void print_error(enum Errors errorId, int fileLine)
{
    char *error_message = getErrorMessage(errorId);
    char *msg = NULL;

    if (fileLine <= -1)
    {
        msg = concatStrings(error_message, "\n");
    }
    else
    {
        char *lineStr = intToString(fileLine);
        char *message1 = concatStrings(error_message, " at line ");
        char *message2 = concatStrings(message1, lineStr);
        msg = concatStrings(message2, "\n");
        free(lineStr);
        free(message1);
        free(message2);
    }

    write(STDERR_FILENO, msg, strlen(msg));
    free(msg);
}