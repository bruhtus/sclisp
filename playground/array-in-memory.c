#include <stdio.h>

/*
 * This is an example of array's memory address.
 * When we passing array, we are passing the memory address of
 * the first index.
 *
 * Array is contiguous with no padding between the
 * items, so we can be sure that the next memory address
 * is part of the array __as long as__ we are within the
 * array size.
 *
 * Reference:
 * https://stackoverflow.com/a/54908906
 */
int main(void)
{
	int arr[3];
	arr[0] = 69;
	arr[1] = 42;
	arr[2] = 69420;

	printf("arr: %p\n", arr);
	printf("arr[0]: %p\n", &(arr[0]));
	printf("arr[1]: %p\n", &(arr[1]));
	printf("arr[2]: %p\n", &(arr[2]));

	return 0;
}
