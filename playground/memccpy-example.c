#include <stdio.h>
#include <string.h>

#define BUF_SIZE 5

/*
 * References:
 * - https://aticleworld.com/memccpy-in-c/
 * - https://developers.redhat.com/blog/2019/08/12/efficient-string-copying-and-concatenation-in-c
 */
int main(void)
{
	void *found;
	char buf[BUF_SIZE];

	char str[] = {'a', 'b', 'c', 'd', '\0'};

	/*
	 * The memccpy() will return null if the provided
	 * character (in this case is '\0') is not found
	 * in the source's characters. The source's
	 * characters searched depends on the size provided
	 * at the last argument.
	 *
	 * For example, if we have the source with
	 * 6 characters and we provide the memccpy() size
	 * with 4 like this:
	 * memccpy(buf, str, '\0', 4);
	 *
	 * memccpy() will return null if `\0` is not found
	 * on the first 4 characters from source.
	 */
	found = memccpy(buf, str, '\0', BUF_SIZE);
	printf("found: %p\n", found);
	printf("buf: %s\n", buf);

	found = memccpy(buf, str, '\0', BUF_SIZE - 2);
	printf("found: %p\n", found);

	/*
	 * We got buffer overflow here because there's no
	 * null terminator in `buf`, so we will stop printing
	 * until we found the null terminator ('\0') in `src`.
	 */
	printf("buf: %s\n", buf);

	return 0;
}
