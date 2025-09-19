#include <stdio.h>
#include "parse_command.h"
int main(int argc, char const *argv[])
{

	while (1)
	{

		char *line = NULL; // Initialize line pointer to NULL for dynamic allocation
		size_t len = 0;	   // Initialize size to 0
		long read;

		read = getline(&line, &len, stdin);
		// Read a line from standard input
		if (read == -1)
		{
			printf("Error\n");
		}
		else
		{
			parse_command(line);
		}

		/* code */
	}
	return 0;
}
