#include <stdio.h>
int main(int argc, char const *argv[])
{

	char buffer[255];
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
			printf("You entered: %s", buffer);
		}

		/* code */
	}
	return 0;
}
