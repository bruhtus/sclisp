#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *msg_malloc(const char *, const char *, unsigned);

int main(void)
{
	char *msg = msg_malloc("Holy C!", __FILE__, __LINE__);
	printf("msg: %s", msg);
	free(msg);

	return 0;
}

/*
 * Reference:
 * https://www.dhairyashah.dev/posts/how-to-get-the-length-of-a-number-in-c/
 */
char *msg_malloc(
	const char *msg,
	const char *filename,
	unsigned line_number
)
{
	/*
	 * We already considered null terminator (\0)
	 * character in len_msg, that's why len_filename
	 * only having strlen().
	 */
	unsigned len_msg = strlen(msg) + 1;
	unsigned len_filename = strlen(filename);
	unsigned len_line_number = floor(log10(line_number)) + 1;

	unsigned limit;
	const char *fmt;

/*
 * Use `-D DEBUG` with gcc to get info filename and
 * line number.
 */
#ifdef DEBUG
	/*
	 * msg (playground/custom-message-with-filename-and-line-number.c:69)\n\0
	 */
	limit = len_msg + 2 + len_filename + 1 + len_line_number + 2;
	fmt = "%s (%s:%u)\n";
#else
	/*
	 * msg\n\0
	 */
	limit = len_msg + 1;
	fmt = "%s\n";
#endif

	printf("message length: %u\n", limit);

	char *str = malloc(limit);

	/*
	 * Looks like snprintf() did not add the new line
	 * character into the calculation result (?).
	 *
	 * If the snprintf size is the same or more than
	 * the limit, that means we might not have enough
	 * space to add null terminator (\0).
	 *
	 * Reference:
	 * https://stackoverflow.com/a/50498477
	 */
	int size_snprintf = snprintf(
		str,
		limit,
		fmt,
		msg,
		filename,
		line_number
	);

	printf("message length snprintf: %d\n", size_snprintf);

	return str;
}
