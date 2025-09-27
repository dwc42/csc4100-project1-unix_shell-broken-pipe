#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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