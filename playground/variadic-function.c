#include <stdio.h>
#include <stdarg.h>

void variadic_print(const char *, ...);

int main(void)
{
	variadic_print("anu %s\n", "itu");

	return 0;
}

void variadic_print(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	vprintf(fmt, ap);
	vprintf(fmt, ap);

	va_end(ap);
}
