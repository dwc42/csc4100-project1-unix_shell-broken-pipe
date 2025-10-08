#include "error_table.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "libraries/string_int.h"
char *getErrorMessage(enum Errors errorId)
{
    switch (errorId)
    {
    case EXEVPFailure:
    {
        return "Execuable Run Failure Error has occured";
    }
    case FileOpenFailure:
    {
        return "File Open Failure Error has occured";
    }
    case ForkFailure:
    {
        return "Fork Failure Error has occured";
    }
    case PathInvalid:
    {
        return "Path Invaild Error has occured";
    }
    case TooManyArgs:
    {
        return "Too Many Arguments Error has occured";
    }
    case TooFewArgs:
    {
        return "Too Few Arguments Error has occured";
    }
    case ValueIsNull:
    {
        return "Value is Null Error has occured";
    }
    case Unknown:
    default:
    {
        return "Unknown Error has occured";
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