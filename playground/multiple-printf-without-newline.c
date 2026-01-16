#include <stdio.h>

void first(void);
void second(void);

/*
 * This will print out only on the last printf() call, the
 * one with new line character (\n), which is interesting.
 *
 * It looks like this behavior where we print out the text
 * when we encounter the new line character (\n) is because
 * the stream that we used, in this case stdout, is a
 * line-buffered stream.
 *
 * Line-buffered stream will print out or _flushed_ out the
 * content to the output device, like shell, via system call,
 * like write() on unix-like OS, when encounter new line
 * character (\n) or the buffer is full (the size of the
 * buffer depends on the system).
 *
 * This make me wonder, even when we use multiple printf(),
 * valgrind still report that we only use 1 memory allocation.
 * Does that mean we use the same memory allocation even
 * though we use multiple printf()? From the reference[4], it
 * looks like printf() using static buffer to store the
 * content and then the content is _flushed_ out when the
 * static buffer full or encounter new line character (\n).
 * In GNU `glibc`, the default static buffer for `stdout` is
 * 8 kb.
 *
 * References:
 * [1] https://softwareengineering.stackexchange.com/a/381789
 * [2] https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_206
 * [3] https://www.cs.purdue.edu/homes/cs240/resources/printf.html
 * [4] https://linuxvox.com/blog/which-c-standard-library-functions-use-malloc-under-the-hood/
 */
int main(void)
{
	first();
	second();
	printf("ini: %d\n", 69420);

	return 0;
}

void first(void)
{
	/*
	 * It looks like when we use fprintf() without new
	 * line character at the end, the text will be print
	 * out when we encounter the text with new line
	 * character.
	 */
	fprintf(stdout, "anu: %d", 69);
}

void second(void)
{
	printf("itu: %d", 42);
}
