int stringcmp(const char *str1, const char *str2);

/*
 * References:
 * - https://stackoverflow.com/a/67520047
 * - https://www.reddit.com/r/learnprogramming/comments/7xzaiv/comment/ducbt4y
 *
 * Strings literals are pointers in C.
 * When you see a sentence like "Hello world!",
 * that variable is of the type const char*,
 * meaning it is a pointer to a bunch of chars that cannot be modified.
 */
int stringcmp(const char *str1, const char *str2)
{
	while (*str1 && *str1 == *str2) {
		str1++;
		str2++;
	}

	int result = (unsigned char)*str1 - (unsigned char)*str2;
	return result;
}
