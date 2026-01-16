#include <stdio.h>
#include <string.h>

int main(void)
{
	char *new_dest;

	char dest = 'A';
	char src_1 = 'B';
	char src_2 = 'C';

	new_dest = memmove(
		&dest,
		&src_1,
		1
	);

	printf("dest value: %c\n", dest);
	printf("dest address: %p\n", &dest);
	printf("new dest address: %p\n", new_dest);

	/*
	 * Looks like when we use 0 as the size, memmove()
	 * will not copy any bytes from source to destination,
	 * basically copying 0 byte.
	 */
	new_dest = memmove(
		&dest,
		&src_2,
		0
	);

	printf("dest value: %c\n", dest);
	printf("dest address: %p\n", &dest);
	printf("new dest address: %p\n", new_dest);

	return 0;
}
