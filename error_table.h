#ifndef __ERROR_TABLE_H
#define __ERROR_TABLE_H

enum Errors {
    Unknown = 0,
    ValueIsNull =1,
    TooManyArgs = 2,
    TooFewArgs = 3,
    PathInvalid = 4,
    ForkFailure = 5,
    FileOpenFailure = 6,
    EXEVPFailure = 7,
};
char *getErrorMessage(enum Errors errorId);
void print_error(enum Errors errorId, int fileLine);
#endif