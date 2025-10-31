#include <stdio.h>

void changed_pointer(int *p);
void changed_value(int *p);

/*
 * This is another example for pointer in a function,
 * other than the dangling pointer example.
 *
 * In this example, we can change the value that the
 * pointer p __points__ to in another function, but if we want
 * to change the pointer address into something else, we
 * need to pass it as a pointer of pointer.
 *
 * Basically we need the memory address of the variable
 * that hold another memory address as a value (in this case,
 * the memory address of variable pointer p). After we got
 * the memory address of the pointer variable, we can change
 * the memory address inside those variable with something
 * else.
 */
int main(void)
{
	int a = 69;
	int *p = &a;
	printf("p pointer stored: %p\n", &p);
	printf("p before: %p\n", p);

	changed_pointer(p);
	printf("p after changed pointer: %p\n", p);

	changed_value(p);
	printf("p after changed value: %d\n", *p);

	return 0;
}

void changed_pointer(int *p)
{
	int a = 42;
	p = &a;
	printf("p change pointer: %p\n", p);
}

void changed_value(int *p)
{
	*p = 42;
	printf("p change value: %d\n", *p);
}
