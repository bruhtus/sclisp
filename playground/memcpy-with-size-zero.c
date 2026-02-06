#include <stdio.h>
#include <string.h>

int main(void)
{
	char *new_dest;

	char dest = 'A';
	char src_1 = 'B';
	char src_2 = 'C';

	new_dest = memcpy(
		&dest,
		&src_1,
		1
	);

	printf("dest value (size non-zero): %c\n", dest);

	/*
	 * Looks like memcpy() also will not copy any bytes
	 * from source to destination if the size is 0,
	 * similar to memmove().
	 */
	new_dest = memcpy(
		&dest,
		&src_2,
		0
	);

	printf("dest value (size zero): %c\n", dest);

	return 0;
}
