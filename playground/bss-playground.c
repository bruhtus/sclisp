#include <stdio.h>

int b[1000000] = {69};

int main()
{
	printf("Size of array: %lu bytes\n", sizeof(b));
	printf("b[1] = %d\n", b[1]);

	return 0;
}
