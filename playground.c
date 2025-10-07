#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
/* #include <histedit.h> */

/* #define max_input 2048 */

/* static char input[max_input]; */

int stringcmp(const char *str1, const char *str2);

int main()
{
	puts("Sclisp v0.0.1-rc1");
	puts("Press ctrl-c to exit\n");

	char prompt[128] = "";

	while (1) {
		/* fputs("sclisp> ", stdout); */
		/* fgets(input, max_input, stdin); */

		sprintf(prompt, "sclisp> ");
		char *input = readline(prompt);

		add_history(input);

		if (input == NULL || stringcmp(input, "exit") == 0) {
			free(input);
			break;
		}

		int where_hist = where_history();
		printf("%d\n", where_hist);

		printf("Holy, C blessing: %s\n", input);
		free(input);
	}

	return 0;
}

/*
 * References:
 * - https://stackoverflow.com/a/67520047
 * - https://www.reddit.com/r/learnprogramming/comments/7xzaiv/comment/ducbt4y
 *
 * Strings literals are pointers in C.
 * When you see a sentence like "Hello world!",
 * that variable is of the type const char*,
 * meaning it is a pointer to a bunch of chars that cannot be modified.
 */
int stringcmp(const char *str1, const char *str2)
{
	/* char p = 'A'; */
	/* printf("%d\n", p); */

	/* printf("%c\n", *str1); */
	/* printf("%s\n", str2); */
	/* printf("%c\n", *(str2 + 1)); */

	/* char *const s1 = "something!"; */
	/* const char *const s1 = "something!"; */
	/* char *s1 = "something!"; */
	const char *s1 = "something!";
	char s2[] = "something";
	s2[0] = 'A';

	printf("%s\n", s2);
	printf("%p\n", s1);

	s1 = "ANU";
	printf("%p\n", s1);

	// The sizeof behave differently.
	printf("s1: %d\n", sizeof(s1)); // Returning size of pointer.
	printf("s2: %d\n", sizeof(s2));

	while (*str1 && *str1 == *str2) {
		/* printf("str1: %d\n", *str1); */
		/* printf("str1 char: %c\n", *str1); */
		str1++; // called pointer arithmetic.
		str2++;
	}

	int result = (unsigned char)*str1 - (unsigned char)*str2;
	return result;
}
