#include <unistd.h>
#include <stdio.h>

#define SOME_ERR(msg) "Error: " #msg "\n"
#define SIZE_SOME_ERR(msg) sizeof(SOME_ERR(msg))

void some_print(const char *, size_t);

/*
 * Reference:
 * https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
 */
int main(void)
{
	some_print(
		SOME_ERR(index out of bounds),
		SIZE_SOME_ERR(index out of bounds)
	);

	printf(
		"%lu\n",
		SIZE_SOME_ERR(index out of bounds)
	);

	printf(
		"macro: %p\n",
		SOME_ERR(index out of bounds)
	);

	printf(
		"string literal: %p\n",
		"Error: index out of bounds\n"
	);

	printf(
		"macro with double quote: %p\n",
		SOME_ERR("index out of bounds")
	);

	return 0;
}

void some_print(const char *msg, size_t len)
{
	write(STDOUT_FILENO, msg, len);
}
