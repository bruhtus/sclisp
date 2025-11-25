#include <stdio.h>

void first(int first_value);
void second(int second_value);

/*
 * Because the state of stack frame for both first() and
 * second() is very similar (?), there's a possibility that
 * the address for local variable on first() and second()
 * is also the same.
 *
 * Currently still not sure about the state of stack frame
 * used in the reference below.
 *
 * Reference:
 * https://stackoverflow.com/a/24113734
 */
int main(void)
{
	printf("first() address: %p\n", &first);
	first(69);

	putchar('\n');

	printf("second() address: %p\n", &second);
	second(42);
}

void first(int first_value)
{
	int a = first_value;
	int b = 69420;

	printf("address of variable a: %p\n", &a);
	printf("value of variable a: %d\n", a);
	printf("address of variable b: %p\n", &b);
	printf("value of variable b: %d\n", b);
}

void second(int second_value)
{
	/*
	 * The value of c is undefined behavior, which means
	 * that the value might be different depending on
	 * the compiler and the system being used.
	 */
	int c;
	int d = second_value;

	printf("address of variable c: %p\n", &c);
	printf("value of variable c: %d\n", c);
	printf("address of variable d: %p\n", &d);
	printf("value of variable d: %d\n", d);
}
