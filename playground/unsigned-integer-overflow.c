#include <stdio.h>
#include <limits.h>

/*
 * The result of unsigned integer overflow is that the
 * actual value modulo the maximum unsigned integer
 * value + 1.
 *
 * Let's say the maximum unsigned integer value is
 * 4294967295, then the second operand for modulo operation
 * is 4294967296.
 *
 * And then the actual value is 4294967298, so the modulo
 * between 4294967298 and 4294967296 (which annotate to
 * 4294967298 % 4294967296) is 2.
 *
 * Reference:
 * - https://phrack.org/issues/60/10
 */
int main(void)
{
	unsigned int result;

	unsigned int i = 0xffffffff; // 4294967295
	printf("no overflow: %u\n", i);

	unsigned int a = UINT_MAX;
	unsigned int b = 1;
	result = a + b;
	printf("overflow 1: %u\n", result);

	unsigned int c = 2;
	result = a + c;
	printf("overflow 2: %u\n", result);

	unsigned int d = 0x100000000; // 4294967296
	printf("overflow 3: %u\n", d);

	/*
	 * 4294967297 % 4294967296
	 */
	unsigned int e = 0x100000001 % 0x100000000;
	printf("overflow 4: %u\n", e);

	/*
	 * 4294967298 % 4294967296
	 */
	unsigned int f = 0x100000002 % 0x100000000;
	printf("overflow 5: %u\n", f);

	unsigned int g = a + a;
	printf("overflow 6: %u\n", g);

	return 0;
}
