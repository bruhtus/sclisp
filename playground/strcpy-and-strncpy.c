#include <stdio.h>
#include <string.h>

int main(void)
{
	char buf[3];
	const char *str = "ab";

	/*
	 * If the source, which is str, has more length than
	 * the destination, which is buf, than there's a
	 * chance for buffer overflow to happen.
	 */
	strcpy(buf, str);
	printf("buf strcpy: %s\n", buf);

	/*
	 * strncpy() is not a safer alternative to strcpy()
	 * because it suffer the same buffer overflow if the
	 * destination length is shorter than the source
	 * length.
	 *
	 * Using strncpy() is like using a fixed-length array
	 * and replacing the elements inside of it. So if
	 * those fixed-length array filled up, strncpy() won't
	 * put the null terminator (\0) at the end of
	 * destination.
	 *
	 * Maybe if we use destination length as the size
	 * instead of source length, we can kind of prevent
	 * the buffer overflow (?), assumming we know the
	 * destination length.
	 *
	 * There's _supposedly_ a safer alternative
	 * called strlcpy() from BSD system.
	 *
	 * References:
	 * - https://stackoverflow.com/a/1258577
	 * - https://randomascii.wordpress.com/2013/04/03/stop-using-strncpy-already/
	 */
	str = "cd";
	strncpy(buf, str, strlen(str) + 1);
	printf("buf strncpy: %s\n", buf);

	buf[0] = '\0';
	str = "ef";
	strncat(buf, str, sizeof(buf) - 1);
	printf("buf strncat: %s\n", buf);

	return 0;
}
