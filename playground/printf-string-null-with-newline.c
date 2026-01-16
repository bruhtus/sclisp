#include <stdio.h>

/*
 * Reference:
 * https://linuxvox.com/blog/adding-newline-character-to-printf-changes-code-behaviour/
 */
int main(void)
{
	char *str1 = NULL;
	char *str2 = NULL;

	/*
	 * If we put \n character at the end of format
	 * format string like "%s\n" when str1 is NULL,
	 * we will get a segmentation fault error. This is
	 * because when we use printf("%s\n", str1), gcc will
	 * optimize that to use puts() instead of printf().
	 *
	 * In gcc, puts() does not have a safety guard to
	 * check whether the pointer is NULL or not, which is
	 * something that printf() has.
	 *
	 * We can check this by using this command to get
	 * the assembly file:
	 * gcc -S playground/printf-string-null-with-newline.c
	 */
	/* printf("%s\n", str1); */

	/*
	 * Looks like gcc only optimize the
	 * printf() with format specifier "%s\n" to use
	 * puts(). So if we add another format specifier like
	 * below, it will use printf() instead of puts().
	 */
	printf("%s %s\n", str1, str2);

	return 0;
}
