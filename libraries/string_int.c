#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
// chatgpt gave concept
// https://chatgpt.com/share/68d81db5-2b28-8004-a302-4416f31ec21f
int stringToInt(char *string, char **endptr)
{
	errno = 0; // reset errno before call

	int num = strtol(string, endptr, 10); // base 10

	if (errno != 0)
	{
		perror("strtol");
		return 1;
	}
	return num;
}

// github copilit
char *intToString(int value)
{
	// Enough for INT_MIN and null terminator
	char buffer[32];
	int len = snprintf(buffer, sizeof(buffer), "%d", value);
	if (len < 0)
		return NULL;
	// Allocate just enough space (+1 for null terminator)
	char *result = malloc(len + 1);
	if (!result)
		return NULL;
	snprintf(result, len + 1, "%d", value);
	return result;
}

char *concatStrings(const char *string1, const char *string2)
{
	int length1 = strlen(string1);
	int length2 = strlen(string2);
	char *output = malloc(length1+ length2+1);
	if (!output) return NULL;
	strcpy(output, string1);
	strcat(output, string2);
	return output;
}