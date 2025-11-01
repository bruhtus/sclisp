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

	str = "nganu";
	printf("str 4: %p\n", str);

	printf(
		"first character: %c, %c, %p\n",
		*(str + 0),
		*str,
		str
	);

	printf(
		"first character ASCII hexadecimal: %p\n",
		*str
	);

	/*
	 * Because we are using *str, it means that we are
	 * accessing one character at a time and keep in mind
	 * that string is an array of characters with
	 * null terminator (\0), and null terminator
	 * can be considered as zero which is false
	 * in conditional check. So the looping below
	 * basically check if the current character is null
	 * terminator or not. And if it's a null terminator,
	 * it won't continue the while loop.
	 *
	 * The str++ means that we are advancing to the next
	 * character, like i++ in for loop.
	 *
	 * Notice how we are printing the string character
	 * in this loop? As we go on the loop, it seems like
	 * the actual string literal get changed until we
	 * found null terminator.
	 */
	while (*str) {
		str++;
		printf(
			"character: %s, pointer: %p, is null terminator? %d\n",
			str,
			str,
			*str == '\0'
		);
	}

	printf("after while loop: %s\n", str);

	return 0;
}
