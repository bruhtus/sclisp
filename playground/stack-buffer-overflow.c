#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Reference:
 * https://phrack.org/issues/60/10
 */
int main(int argc, char **argv)
{
	unsigned short s;
	int n;
	char buf[65535];

	printf("argv[0]: %s\n", argv[0]);

	if (argc < 3) {
		printf("Error: need 3 arguments!\n");
		return -1;
	}

	n = atoi(argv[1]);
	s = n;

	printf("n: %d\n", n);
	printf("s: %hu\n", s);
	printf("argv[2]: %c\n", argv[2][0]);
	printf("length argv[2]: %d\n", strlen(argv[2]) + 1);
	printf("length buf: %d\n", sizeof(buf));

	/*
	 * This safety guard will be ignored if we exceed
	 * the maximum value of unsigned short type, which
	 * we can check on USHRT_MAX in limits.h.
	 */
	if (s > 80) {
		printf("Error: more than 80 characters!\n");
		return -1;
	}

	/*
	 * If the string we are trying to copy is less than
	 * the size we provide, we might get segmentation
	 * fault because we are trying to access a memory
	 * that we have no permission to access.
	 *
	 * For example, let's say we provide the size of 5000
	 * just to copy string "hello" which only have
	 * 6 bytes (because char only have 1 byte
	 * and we have 6 chars:
	 * {'h', 'e', 'l', 'l', 'o', '\0'}) like this:
	 * memmove(buf, "hello", 5000);
	 *
	 * The example above will make us reading the memory
	 * outside the string "hello" and when we get into
	 * memory that we have permission, we will get
	 * segmentation fault.
	 *
	 * Some people can exploit this using the stack buffer
	 * overflow. For more info on stack buffer overflow:
	 * https://en.wikipedia.org/wiki/Stack_buffer_overflow
	 *
	 * Some compiler, like gcc, will give a warning about
	 * stringop-overread.
	 *
	 * We can kind of reproduce the overread scenario
	 * with this snippet:
	 * int i;
	 * for (i = 0; i < n; i++) {
	 * 	printf(
	 * 		"argv[2] char: %c, i: %d\n",
	 * 		argv[2][i],
	 * 		i
	 * 	);
	 * 	buf[i] = argv[2][i];
	 * }
	 *
	 * This can happen using memcpy() and memmove().
	 *
	 * For the definition of how many 1 byte is,
	 * check CHAR_BIT in limits.h because 1 byte might
	 * not be 8 bits in some machine.
	 */
	memmove(buf, argv[2], n);

	printf("buf result: %s\n", buf);

	return 0;
}
