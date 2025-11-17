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

	/*
	 * The snipet below is different from
	 * char *itu = "anu";
	 * because itu variable points to anu array variable
	 * which is saved in the stack. While literal string,
	 * use the double quote like "anu" is usually saved
	 * in read-only segments of memory (depends on
	 * implementation of the compiler).
	 *
	 * Here's the gist of it:
	 * - anu variable is saved in the stack.
	 * - itu is a pointer to anu variable memory address.
	 * - We can change the character of itu variable
	 *   because the data is saved in the stack, instead
	 *   of in the read-only segments of memory.
	 */
	char anu[] = "anu";
	char *itu = anu;
	itu[0] = 'i';

	/*
	 * If we change the memory address in pointer itu
	 * to string literal "itu", we can't change the
	 * character because now itu variable points to
	 * read-only segments of memory.
	 *
	 * So if we do
	 * itu[0] = 'a';
	 * after the statement below, we will get segmentation
	 * fault because we are trying to access memory we
	 * don't have access.
	 *
	 * The only way to change the value is replacing the
	 * __memory address__ in pointer itu with something
	 * else.
	 */
	itu = "itu";

	printf("itu: %s\n", itu);

	return 0;
}
