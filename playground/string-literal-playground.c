#include <stdio.h>

void change_string1(const char **str);
void change_string2(char **str);
char (*pointer_array(void))[4];

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

	change_string1(&str);
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
	printf("itu (similar to str 2): %p\n", itu);

	/*
	 * It seems like using the same string literal
	 * in another function will result in using the same
	 * memory address.
	 *
	 * Reference:
	 * https://stackoverflow.com/a/349030
	 */
	change_string2(&itu);
	printf("itu (similar to str 4): %p\n", itu);

	putchar('\n');

	/*
	 * It seems like we can use the string literals memory
	 * address explicitly like `&"abd"`, but i am still
	 * not sure what that means. What is the difference
	 * between using `&"abd"` and `"abd"` string literal
	 * directly?
	 *
	 * I mean, we have a different type when declaring
	 * the string literal as is, like this:
	 * char *confusion = "abd";
	 *
	 * And this:
	 * char (*confusion)[4] = &"abd";
	 *
	 * The second one have type `char (*)[4]` but the
	 * first one have type `char *`.
	 *
	 * What does the difference between `char (*)[4]` and
	 * `char *`?
	 *
	 * My hypothesis is that `char (*)[4]` is similar to
	 * 2-dimensional array with the column of 4 (?). Still
	 * not sure, need more info.
	 *
	 * Reference:
	 * https://stackoverflow.com/questions/9970295/life-time-of-a-string-literal-in-c#comment12737771_9970305
	 */
	char (*confusion)[4] = &"abd";
	printf("confusion: %s\n", confusion);
	printf("confusion: %p\n", confusion);
	printf("confusion: %p\n", &"abd");
	printf("confusion: %p\n", "abd");
	printf("confusion[0][0]: %p\n", confusion[0][0]);
	printf("confusion[0][0]: %c\n", confusion[0][0]);
	printf("confusion[0][1]: %c\n", confusion[0][1]);
	printf("confusion[0][2]: %c\n", confusion[0][2]);
	printf("(*confusion)[2]: %c\n", (*confusion)[2]);
	printf(
		"null terminator confusion: %d\n",
		confusion[0][3] == '\0'
	);

	/*
	 * The expression below produce the same result,
	 * which is a __buffer overflow__ on the line
	 * printf("confusion: %s\n", confusion);
	 *
	 * That's why the result is `c` character, because of
	 * "confusion: %s\n". When we get the value from
	 * confusion[1][11], we got `%` character because `%`
	 * is the 12th elements of `confusion: %s\n`.
	 */
	printf("*confusion[1]: %c\n", *confusion[1]);
	printf("*(confusion[1]): %c\n", *(confusion[1]));
	printf(
		"*(*(confusion + 1)): %c\n",
		*(*(confusion + 1))
	);
	printf("confusion[1][0]: %c\n", confusion[1][0]);
	printf("confusion[1][11]: %c\n", confusion[1][11]);

	/*
	 * This is similar with the `confusion` variable
	 * above, the difference is that we are returning
	 * the value from another function.
	 */
	char (*another_confusion)[4] = pointer_array();
	printf(
		"another_confusion: %s\n",
		another_confusion
	);

	return 0;
}

void change_string1(const char **str)
{
	*str = "itu";
}

void change_string2(char **str)
{
	*str = "nganu";
}

/*
 * This function return a memory address to
 * array of char with size 4,
 * `{'c', 'e', 'f', '\0'}`.
 *
 * So if we have more character than 4, this
 * function will throw an error at compile-time.
 *
 * Reference:
 * https://stackoverflow.com/a/11657653
 */
char (*pointer_array(void))[4]
{
	return &"cef";
}
