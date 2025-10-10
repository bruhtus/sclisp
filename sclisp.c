#include "stringcmp.h"

#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>

int main()
{
	puts("Sclisp v0.0.1-rc1");
	puts("Press ctrl-c, ctrl-d, exit command to exit\n");

	char prompt[128] = "";

	while (1) {
		sprintf(prompt, "sclisp> ");
		char *input = readline(prompt);

		add_history(input);

		if (input == NULL || stringcmp(input, "exit") == 0) {
			free(input);
			break;
		}

		printf("Holy C blessing: %s\n", input);
		free(input);
	}

	return 0;
}
