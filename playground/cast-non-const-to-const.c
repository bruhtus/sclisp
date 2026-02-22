#include <stdio.h>

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

	return 0;
}
