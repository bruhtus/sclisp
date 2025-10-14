#include <stdio.h>

// Ref: https://stackoverflow.com/a/10349610
void printSizeof(int array[]);
void printLength(int array[]);

int main()
{
	int array[] = {1, 2, 3};

	printf("sizeof array: %d\n", sizeof(array));
	printSizeof(array);

	// Devide size of the array by the size of array element type.
	printf(
		"length array: %d\n",
		(sizeof(array) / sizeof(array[0]))
	);
	printLength(array);
}

// Using *array instead of array[] prevent gcc warning -Wsizeof-array-argument.
void printSizeof(int *array)
{
	printf("sizeof parameter: %d\n", sizeof(array));
}

void printLength(int *array)
{
	printf("sizeof array type: %d\n", sizeof(array[0]));
	printf("sizeof int type: %d\n", sizeof(int));

	printf(
		"length parameter: %d\n",
		(sizeof(array) / sizeof(array[0]))
	);
}

// vim: set noet sw=8 ts=8:
