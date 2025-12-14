#include <stdio.h>

void print_const(const int *start, const int *stop);
void increase(void *data, int p_size);

/*
 * References:
 * - https://cplusplus.com/doc/tutorial/pointers/
 * - https://boredzo.org/pointers/
 */
int main(void)
{
	int num[5];
	int *p = num;

	printf("&num[0]: %p\n", &num[0]);
	printf("p: %p\n", p);
	putchar('\n');

	*p = 10;
	printf("*p: %d\n", *p);
	printf("num[0]: %d\n", num[0]);
	putchar('\n');

	/*
	 * We need the bracket around *p because of
	 * __operator precedence__ rules.
	 *
	 * Operator precedence rules make the
	 * postfix operator (like ++ operator) a higher
	 * priority then the dererefence operator.
	 *
	 * That's why the expression *p++ is equivalent to
	 * *(p++), which means that we dereference unincrement
	 * address first and then increment the pointer.
	 */
	(*p)++;
	printf("*p: %d\n", *p);
	printf("num[0]: %d\n", num[0]);
	printf("*p++: %d\n", *p++);
	putchar('\n');

	*p = 20;
	printf("*p: %d\n", *p);
	printf("num[1]: %d\n", num[1]);
	putchar('\n');

	p = &num[2];
	*p = 30;
	printf("*p: %d\n", *p);
	printf("num[2]: %d\n", num[2]);
	putchar('\n');

	p = num + 3;
	*p = 40;
	printf("*p: %d\n", *p);
	printf("num[3]: %d\n", num[3]);
	putchar('\n');

	/*
	 * Square brackets like [] can mean
	 * __offset operator__, basically this means that we
	 * are dereferencing the pointer with the offset
	 * provided.
	 *
	 * So when we have num[5], we are dereferencing the
	 * pointer num with offset 5. It's similar to
	 * *(num+5).
	 */
	p = num;
	*(p+4) = 50;
	printf("*(p+4): %d\n", *(p+4));
	printf("num[4]: %d\n", num[4]);
	printf("*(num+4): %d\n", *(num+4));
	putchar('\n');

	/*
	 * This is an example of dereferencing a pointer with
	 * brackets.
	 */
	int anu = 69;
	int *itu = &anu;
	printf("itu[0]: %d\n", itu[0]);
	printf("*itu: %d\n", *itu);
	putchar('\n');

	char *my_char;
	short *my_short;
	long *my_long;

	printf("my_char: %ld\n", my_char);
	printf("my_short: %ld\n", my_short);
	printf("my_long: %ld\n", my_long);

	/*
	 * This will print different result because by
	 * adding the number to a pointer, we made the pointer
	 * points to the
	 * __following elements of the same type__.
	 *
	 * To get the _following elements_ of the same type,
	 * we depends on the size of type (which we can get
	 * with sizeof operator).
	 */
	printf("++my_char: %ld\n", ++my_char);
	printf("++my_short: %ld\n", ++my_short);
	printf("++my_long: %ld\n", ++my_long);
	putchar('\n');

	/*
	 * We can assign non-constant memory address to
	 * constant pointer, but for safety reason, when we
	 * assign the memory address of const to non-const,
	 * we will get a warning from the gcc (compiler).
	 */
	const int x = 42;
	const int y = 69;
	int *q = (int *)&y;
	*q = 42;
	printf("*q: %d\n", *q);
	putchar('\n');

	/*
	 * This can cause some problem if the memory address
	 * of x and y is not contiguous or side by side because
	 * we might try to access some memory that we does not
	 * exist or not having the access to.
	 */
	print_const(&x, &y);
	putchar('\n');

	/*
	 * Because pointer and array behave essentially the
	 * same way in __expression__, we can access the
	 * character in string literals in the same way as
	 * array of characters with null terminated character
	 * at the end.
	 */
	const char *holy = "Holy C!";
	printf("holy[3]: %c\n", holy[3]);
	printf("*(holy+3): %c\n", *(holy+3));
	putchar('\n');

	void *holy_void;
	printf("sizeof holy_void: %d\n", sizeof(*holy_void));

	increase(p, sizeof(*p));
	printf("p increase: %d\n", *p);

	char a_char = 'a';
	increase(&a_char, sizeof(a_char));
	printf("a_char increase: %c\n", a_char);

	/*
	 * (*p_func) is called "function pointer", basically
	 * we can assign another function and use function
	 * pointer like those function.
	 */
	void (*p_func)(void *data, int p_size) = increase;
	(*p_func)(p, sizeof(*p));
	printf("p_func increase: %d\n", *p);

	/*
	 * We can also provide function pointer as an array,
	 * and then call the function pointer with
	 * the index or offset number, like (*array[0])(...).
	 */
	void (*p_func_array[])(void *, int) = { increase };
	(*p_func_array[0])(p, sizeof(*p));
	(**p_func_array)(p, sizeof(*p));
	printf("p_func_array increase: %d\n", *p);

	/*
	 * We can also assign function pointer with casting
	 * like below.
	 *
	 * Seems like declaring function pointer is the same
	 * as declaring variable, which means we can't have
	 * multiple function pointer with __the same name__.
	 */
	void (*p_func_cast)(void *, int);
	p_func_cast = (void (*)(void *, int))increase;
	(*p_func_cast)(p, sizeof(*p));
	printf("p_func_cast increase: %d\n", *p);

	return 0;
}

/*
 * We can increment the pointer value itself, but we can not
 * increment the value that __the pointer points to__. In
 * this case, constant is for the value that __the
 * pointer points to__ rather than the pointer value itself.
 */
void print_const(const int *start, const int *stop)
{
	const int *current = start;

	printf("start: %ld\n", start);
	printf("stop: %ld\n", stop);

	while (current <= stop) {
		printf("current: %p\n", current);
		printf("*current: %d\n", *current);
		current++;
	}
}

/*
 * We can passing any pointer type in data parameter because
 * we are using `void *` (a.k.a void pointer) as the data
 * type, but the problem is that we can not directly
 * dereference void pointer. We need to transformed the
 * void pointer into the existing type pointer, like `int *`
 * or `char *`.
 */
void increase(void *data, int p_size)
{
	switch (p_size) {
		case sizeof(char):
			char *p_char = (char *)data;
			printf("p_char: %d\n", *p_char);
			++(*p_char);
			printf("p_char: %d\n", *p_char);
			break;

		case sizeof(int):
			int *p_int = (int *)data;
			++(*p_int);
			break;
	}
}
