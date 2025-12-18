#include <stdio.h>
#include <stdarg.h>

void variadic_print(const char *, ...);
void variadic_process(const char *, va_list);

/*
 * Reference:
 * https://stackoverflow.com/a/37789384
 */
int main(void)
{
	/*
	 * This is an example to trigger the bad scenario.
	 * Still not sure why using 2 variadic arguments
	 * work, but not using 1 variadic argument.
	 */
	variadic_print("%s %s\n", "anu", "itu");

	return 0;
}

void variadic_print(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	variadic_process(fmt, ap);

	va_end(ap);
}

/*
 * To run the bad example, we can use this command:
 * gcc -DBAD -o playground/variadic-function playground/variadic-function.c
 *
 * The BAD is an example of using variadic arguments
 * multiple times, which can result in
 * __undefined behavior__.
 *
 * To prevent that, we can copy the variadic arguments and
 * use the copy value instead.
 */
void variadic_process(const char *fmt, va_list ap)
{
#ifdef BAD
	vprintf(fmt, ap);
#else
	va_list apc;
	va_copy(apc, ap);
	vprintf(fmt, apc);
	va_end(apc);
#endif

	vprintf(fmt, ap);
}
