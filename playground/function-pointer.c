#include <stdio.h>

/*
 * References:
 * - https://www.geeksforgeeks.org/c/function-pointer-in-c/
 * - https://youtu.be/f_uWOWViYc0?t=521
 */
int (*select_op(int op))(int a, int b);
int add(int a, int b);
int minus(int a, int b);

int main(void)
{
	int (*fptr)(int, int);

	fptr = add;
	/* fptr = &add; */

	printf("fptr: %d\n", fptr(1, 2));
	printf("fptr: %d\n", (*fptr)(1, 2));

	int (*fptr_op)(int, int) = select_op(2);
	printf("fptr_op: %d\n", (*fptr_op)(2, 1));

	/*
	 * In the line below, we execute the select_op()
	 * and then execute the returned function pointer
	 * with argument (1, 2).
	 */
	int result = (*select_op(2))(1, 2);
	printf("result: %d\n", result);

	fptr_op = select_op(3);
	if (fptr_op == NULL) {
		printf("fptr_op is NULL\n");
	}

	return 0;
}

/*
 * We can declare a function that __return__ a function
 * pointer.
 *
 * In the example below, we declare a function called
 * `select_op`() with parameter `op` and return a memory
 * address of another function that has parameter
 * (int, int), like add() and minus(). What matter is the
 * parameter type and not the parameter name.
 *
 * For other examples, check file
 * `playground/pointer-and-array.c`.
 */
int (*select_op(int op))(int a, int b)
{
	switch (op) {
		case 1:
			return add;

		case 2:
			return minus;

		default:
			printf("Unrecognized option %d\n", op);
			break;
	}

	return NULL;
}

int add(int a, int b)
{
	return a + b;
}

int minus(int a, int b)
{
	return a - b;
}
