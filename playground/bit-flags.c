/*
 * Reference:
 * https://stackoverflow.com/a/49353436
 */
#include <stdint.h>
#include <stdio.h>

#define O_ORDERDIR 0x01 // 0000 0001
#define O_ORDERTIME 0x02 // 0000 0010
#define O_ORDERICASE 0x04 // 0000 0100

int main(void)
{
	/*
	 * Make sure that unsigned integer is at least 8 bits.
	 */
	uint8_t flags = O_ORDERDIR | O_ORDERICASE;

	if (flags & O_ORDERDIR)
		puts("Order by directory");

	if (flags & O_ORDERTIME)
		puts("Order by time modified");

	if (flags & O_ORDERICASE)
		puts("Order by insensitive case");

	return 0;
}
