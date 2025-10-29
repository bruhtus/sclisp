#include <stdio.h>

/*
 * Seems like initializing using char *str = "..." means that
 * we are putting the pointer of a string literal into `str`
 * variable (?).
 *
 * In the example below, when we assign new string literal,
 * the pointer of `str` is changed to new pointer. But, if
 * we assign the previous string literal, the pointer of `str`
 * is the same as the previous pointer.
 *
 * Basically every string literal has their own pointer (?).
 * Might need to figure this out later.
 */
int main(void)
{
	const char *str = "anu";
	printf("str 1: %p\n", str);

	str = "itu";
	printf("str 2: %p\n", str);

	str = "anu";
	printf("str 3: %p\n", str);

	str = "an";
	printf("str 4: %p\n", str);

	return 0;
}
