#include <stdio.h>

void something(const char *);

/*
 * Casting const to non-const might have some _hidden risk_, but it looks
 * like casting from non-const to const is fine.
 *
 * References:
 * - https://www.gnu.org/software/c-intro-and-ref/manual/html_node/const.html
 * - https://www.w3tutorials.net/blog/c-c-changing-the-value-of-a-const/
 */
int main(void)
{
	char cmd[] = "wc";

	const char *str = cmd;
	printf("str: %s\n", str);

	/*
	 * Looks like we can passing non-const value
	 * to const parameter in a function, but _not_
	 * vice versa.
	 */
	something(cmd);

	return 0;
}

void something(const char *str)
{
	printf("something: %s\n", str);
}
