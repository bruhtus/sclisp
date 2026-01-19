#include <stdio.h>

/*
 * References:
 * - https://www.geeksforgeeks.org/c/function-pointer-in-c/
 * - https://youtu.be/f_uWOWViYc0?t=521
 */
int (*select_op(int op))(int a, int b);
int add(int a, int b);
int minus(int a, int b);

/*
 * typedef introduces an __alias__ for the type, and in this
 * case the alias is `ptr_td` with the type
 * `int (*)(int, int)`.
 *
 * So basically `ptr_td` is an alias for type
 * `int (*)(int, int)`.
 *
 * Reference:
 * https://stackoverflow.com/a/1591492
 */
typedef int (*ptr_td)(int, int);
ptr_td select_op_td(int op);

struct some_struct {
	int (*func)(int, int);
};

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

	/*
	 * fptr_op_td is kind of the same as fptr_op, which
	 * means that the fptr_op_td is also a function
	 * pointer.
	 */
	ptr_td fptr_op_td = select_op_td(2);
	printf("fptr_op_td: %d\n", fptr_op_td(2, 1));

	struct some_struct var = {.func = add};
	printf("var: %d\n", var.func(2, 1));

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

ptr_td select_op_td(int op)
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
