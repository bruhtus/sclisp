#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("Error: need 3 arguments!\n");
		return -1;
	}

	int n = atoi(argv[1]);
	char buf[65535];

	printf(
		"unsigned integer representation of n: %u\n",
		n
	);

	/*
	 * Because memmove() last argument is using size_t
	 * which is unsigned integer type, if we give a
	 * negative input like -69, those value will turn
	 * into unsigned integer representation. This can
	 * create __buffer overflow__ as the size we provide
	 * might be bigger than the actual string length.
	 *
	 * Keep in mind that size_t implementation might
	 * differ for each machine, so treat it size_t as
	 * "unsigned" type instead of literal
	 * "unsigned integer". For example, on 64-bit system,
	 * the value of size_t might be the maximum of
	 * unsigned long long integer type (ULLONG_MAX on
	 * on limits.h) instead of unsigned integer.
	 */
	memmove(buf, argv[2], n);

	printf("buf result: %s\n", buf);

	return 0;
}
