/*
 * Reference:
 * https://youtu.be/gVaXLlGqQ-c
 *
 * To include shared object in binary, we can use -static flag
 * like this:
 * gcc -static -o playground/printf-without-stdlib playground/printf-without-stdlib.c
 */
#include <unistd.h>

int main(void)
{
	const char *holy = "Holy C!\n";

	write(STDOUT_FILENO, holy, sizeof(holy));
	return 0;
}
