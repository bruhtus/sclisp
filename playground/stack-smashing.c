#include <stdio.h>

int main(void)
{
	/*
	 * This will cause a "stack smashing" scenario because
	 * we are trying to access the memory outside of the
	 * allocated memory.
	 *
	 * Because we declare the array with 0 length,
	 * we can not access the next memory, which is
	 * the index 1 (current memory with offset 1). So
	 * this is kind of like array _out of bounds_ scenario
	 * in another programming language (?).
	 *
	 * But for some reason, we can assign a value in the
	 * index 0 of the array with _0 length_ with `gcc`
	 * compiler. This is one of those
	 * _undefined behavior_, so there's _no guarantee_
	 * that this will always be the case.
	 *
	 * Some compiler will not give any warning or
	 * error with declaring 0 length array.
	 * Like using `gcc` without flag `-pedantic` or
	 * `-pedantic-errors`.
	 *
	 * We can get more info about the "stack smashing" by
	 * using flag `-fsanitize=address` with `gcc`,
	 * which will give us some information about the
	 * memory error.
	 *
	 * References:
	 * - https://stackoverflow.com/a/40416567
	 * - https://en.wikipedia.org/wiki/Stack_buffer_overflow
	 * - https://stackoverflow.com/q/9722632
	 * - https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Zero-Length.html
	 * - https://stackoverflow.com/a/15647634
	 */
	int arr[0];
	printf("sizeof arr: %d\n", sizeof(arr));

	arr[0] = 69;
	arr[1] = 42;

	return 0;
}
