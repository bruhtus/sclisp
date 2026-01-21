#include <stdio.h>
#include <string.h>

size_t stringlcpy(char *, const char *, size_t);

/*
 * Reference:
 * https://github.com/freebsd/freebsd-src/blob/main/lib/libc/string/strlcpy.c
 */
int main(void)
{
	char buf[4];
	const char *str = "ab";

	/*
	 * This is safer than strncpy() because strlcpy()
	 * guarantee to have null terminator at the end of
	 * the result, __but__ if the size provided is more
	 * than the size of the destination, we still trigger
	 * buffer overflow.
	 */
	strlcpy(buf, str, strlen(str) + 1);
	printf("buf: %s\n", buf);

	/*
	 * Check how stringlcpy() behave if we provided the
	 * size without null terminator.
	 */
	str = "cde";
	size_t result = stringlcpy(buf, str, strlen(str));
	printf("buf stringlcpy: %d\n", buf[0] == '\0');
	printf("buf stringlcpy: %s\n", buf);
	printf("result: %lu\n", result);

	return 0;
}

size_t stringlcpy(char *dst, const char *src, size_t size)
{
	const char *osrc = src;
	size_t nleft = size;

	if (nleft != 0) {
		printf("size: %lu\n", size);

		/*
		 * We exit the looping when either we hit
		 * null terminator (\0) from the src first
		 * or before the leftover (nleft) become 0
		 * by decrement (when size - 1).
		 */
		while (--nleft != 0) {
			*dst = *src;

			printf("nleft: %lu\n", nleft);
			printf("*dst: %d\n", *dst == '\0');

			if (*dst == '\0')
				break;

			++dst;
			++src;
		}
	}

	/*
	 * If we exit the previous looping when the leftover
	 * (nleft) _become_ 0, we add the null terminator on
	 * the destination's memory address if the _provided_
	 * size is not 0.
	 */
	if (nleft == 0) {
		if (size != 0) {
			printf("size 0\n");
			*dst = '\0';
		}

		printf("src before: %c\n", src[0]);

		/*
		 * This will get triggered when the provided
		 * size did not include null terminator (\0).
		 */
		while (*src)
			++src;

		printf("src after: %d\n", src[0]);
	}

	/*
	 * Does not return null terminator (\0).
	 */
	return (src - osrc - 1);
}
