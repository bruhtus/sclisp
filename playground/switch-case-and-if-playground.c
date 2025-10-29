#include <stdio.h>

int main(void)
{
	/*
	 * We can not use type other than integer for switch
	 * expression (the one in the parenthesis). Below is
	 * the example of not supported switch expression.
	 */
	/* float anu = 69.420f; */
	/* char *anu = "something"; */

	/* int anu = 69; */
	/* long anu = 69; */
	char anu = 'A'; // Equal 65 in ASCII character set.

	/*
	 * switch is different from if, where if can take any
	 * expression and as long as it's not zero, then the
	 * statement in if body will be executed.
	 */
	switch (anu) {
		default:
			printf("not recognized\n");
			break;

		case 69:
			printf("nice\n");
			break;

		case 41 + 1:
			printf("life\n");
			break;
	}

	/* char *itu = NULL; */
	char itu = '\001'; // Octal number of start of heading on ASCII character set (non-printable).

	/*
	 * If itu is NULL, the statement inside of if will not
	 * be executed because NULL is equal to 0 in ASCII
	 * character set.
	 */
	if (itu)
		printf("printed if\n");

	return 0;
}
