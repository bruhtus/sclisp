#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Reference:
 * https://stackoverflow.com/a/12918541
 */
int main(void)
{
	int i;

	/*
	 * Let's say we previously create array with size 2,
	 * and then we decided to resize those array because
	 * we want to add another element inside of it.
	 */
	int arr[2] = {69, 42};
	printf("arr[1]: %d\n", arr[1]);

	/*
	 * We create new array with the new size that is
	 * double the previous array length.
	 */
	size_t prev_arr_len = sizeof(arr) / sizeof(arr[0]);

	int *new_arr = malloc(
		sizeof(*new_arr) * (prev_arr_len * 2)
	);

	/*
	 * We copy all the elements in `arr` to `new_arr`.
	 */
	memmove(
		new_arr,
		&arr,
		sizeof(arr)
	);

	new_arr[2] = 69420;

	/*
	 * We copy the element of index 1 `arr` into index 3
	 * `new_arr`.
	 */
	memmove(
		new_arr + 3,
		arr + 1,
		sizeof(arr[1])
	);

	for (i = 0; i < (prev_arr_len * 2); ++i)
		printf("new_arr[%d]: %d\n", i, new_arr[i]);

	putchar('\n');

	/*
	 * Now we try to resize the new_arr with the size
	 * that is double of the previous size.
	 */
	size_t realloc_size = prev_arr_len * 2 * 2;

	new_arr = realloc(
		new_arr,
		realloc_size
	);

	if (new_arr == NULL && realloc_size != 0) {
		printf("realloc failed\n");
		return 69;
	}

	new_arr[4] = 42690;
	new_arr[5] = 4269;
	new_arr[6] = 1234;
	new_arr[7] = 4321;

	for (i = 0; i < realloc_size; ++i)
		printf("new_arr[%d]: %d\n", i, new_arr[i]);

	free(new_arr);

	return 0;
}
