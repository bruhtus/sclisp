#include <stdio.h>

/* int b[1000000] = {69}; */
/* int b[1000000]; */

/*
 * For global function, check this:
 * https://stackoverflow.com/q/28634427
 */
int main()
{
	/*
	 * There's a difference between initialization the
	 * static or global value with declaring and
	 * assigned the value afterward.
	 *
	 * If we initialize the static or global value,
	 * like example below, we are putting the value
	 * in the binary. So the binary size might increase
	 * if we have a lot of value.
	 */
	/* static int b[1000000] = {69}; */

	/*
	 * If we declare and assign the value later like this,
	 * the binary only contains the info about how much
	 * memory to allocate and assignment of the value
	 * will happen in the run-time.
	 *
	 * Reference:
	 * https://chessman7.substack.com/p/understanding-the-bss-segment-in
	 */
	static int b[1000000];
	b[0] = 69;

	printf("Size of array: %lu bytes\n", sizeof(b));
	printf("b[0] = %d\n", b[0]);
	printf("b[1] = %d\n", b[1]);

	return 0;
}
