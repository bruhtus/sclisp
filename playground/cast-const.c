#include <stdio.h>

/*
 * Casting const type to non-const type can have a hidden risk,
 * one of them is accidental modification.
 *
 * Reference:
 * https://www.w3tutorials.net/blog/c-c-changing-the-value-of-a-const/
 */
int main(void)
{
	const char *x = "ab";

	char *ptr_x = (char *)x;

	/*
	 * This is __undefined behavior__.
	 */
	ptr_x[0] = 'c';

	printf(
		"x: %s, *ptr_x: %c\n",
		x,
		*ptr_x
	);

	return 0;
}
