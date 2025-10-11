#include <stdio.h>
#include <string.h>

// Ref: https://www.reddit.com/r/C_Programming/comments/rlzncv/comment/hpkv3tz
int main()
{
	/* Ref: https://www.reddit.com/r/cprogramming/comments/1caugid/comment/l0ubwj4
	 *
	 * Normally a string literal is converted to a pointer, but when a string literal appears as an initializer for array of char (like char a[] = "abc"), the string literal get copied into the array. So we only able to use initialization for the array of char and not separate declaration and assignment (?).
	 */

	char a1[3] = "ab\0";
	char a2[4] = "defg";
	char b1[] = "hijk";
	char b2[] = "lmn";
	char *c1 = "opq";
	char *c2 = "rstu";

	printf("a1: %s is size %d\n", a1, sizeof(a1));
	printf("a2: %s is size %d\n", a2, sizeof(a2));
	printf("b1: %s is size %d\n", b1, sizeof(b1));
	printf("b2: %s is size %d\n", b2, sizeof(b2));
	printf("c1: %s is size %d\n", c1, sizeof(c1));
	printf("c2: %s is size %d\n", c2, sizeof(c2));

	return 0;
}
