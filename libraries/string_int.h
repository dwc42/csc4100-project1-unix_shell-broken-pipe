#ifndef __STRING_INT_H
#define __STRING_INT_H

int stringToInt(char *string, char **endptr);
char *intToString(int value);
char *concatStrings(const char *string1, const char *string2);
#endif