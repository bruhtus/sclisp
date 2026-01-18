#include "utils.h"

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * It seems like putting macro in header file can result
 * in unexpected behavior and make it harder to find where
 * the macro came from.
 *
 * Reference:
 * https://stackoverflow.com/a/30743782
 */
#define MAX_BIT_SIZE(type) ( \
	(sizeof(type) * CHAR_BIT) - 1 \
)

/*
 * Check whether we are exceeding the PTRDIFF_MAX when using
 * size_t.
 *
 * Currently this is only print out the overflow value for
 * signed integer (int, long, long long). So if we provide
 * the unsigned integer type, like `unsigned int`, we will
 * get the value of INT_MAX + 1.
 *
 * References:
 * - https://lteo.net/blog/2014/10/28/reallocarray-in-openbsd-integer-overflow-detection-for-free/
 * - http://cvsweb.openbsd.org/cgi-bin/cvsweb/~checkout~/src/lib/libc/stdlib/reallocarray.c
 */
#define OVERFLOW_VALUE(type) ( \
	(size_t)1 << MAX_BIT_SIZE(type) \
)

#define MALLOC_ERR_MSG "malloc failed"
#define REALLOC_ERR_MSG "realloc failed"

/*
 * Static variable or function must be
 * resolved at compile-time, so we can't use
 * another variable or return value of another
 * function call.
 *
 * References:
 * - https://software.codidact.com/posts/285050
 * - https://web.archive.org/web/20080624102132/http://www.space.unibe.ch/comp_doc/c_manual/C/CONCEPT/storage_class.html
 * - https://web.archive.org/web/20080624094524/http://www.space.unibe.ch/comp_doc/c_manual/C/SYNTAX/static.htm
 * - https://stackoverflow.com/a/2929077
 */
static void lval_expr_print(
	struct lval *v,
	char open,
	char close
);
static void lval_print(struct lval *value);

struct lval *lval_eval(struct lval *value)
{
	if (value->type == LVAL_SEXPR)
		return lval_eval_sexpr(value);

	return value;
}

struct lval *lval_eval_sexpr(struct lval *value)
{
	int i;

	/*
	 * Prevent integer overflow when we got too many
	 * expression. Still not sure if we should larger
	 * integer type for value->count.
	 *
	 * Keep in mind that this method is tested on 64-bit
	 * system with char has 1 byte size, so this might not
	 * work on other system.
	 *
	 * And also, this method only tested on the
	 * signed integer, so this method might not work
	 * for unsigned integer where the overflow value is
	 * defined.
	 *
	 * References:
	 * - http://www.fefe.de/intof.html
	 * - https://stackoverflow.com/a/21650326
	 * - https://www.programiz.com/c-programming/bitwise-operators
	 * - https://stackoverflow.com/a/141873 (right or left shift operator).
	 */
	if ((value->count >> MAX_BIT_SIZE(value->count)) != 0) {
		lval_del(value);
		return lval_err(
			"value->count got integer overflow",
			__FILE__,
			__LINE__
		);
	}

	if (value->count == 0)
		return value;

	for (i = 0; i < value->count; i++) {
		value->cell[i] = lval_eval(value->cell[i]);

		if (value->cell[i]->type == LVAL_ERR)
			return lval_take(value, i);
	}

	if (value->count == 1)
		return lval_take(value, 0);

	struct lval *first = lval_pop(
		value,
		0,
		__FILE__,
		__LINE__
	);

	if (first->type == LVAL_ERR)
		return first;

	if (first->type != LVAL_SYM) {
		lval_del(first);
		lval_del(value);
		return lval_err(
			"s-expression does not start with symbol",
			__FILE__,
			__LINE__
		);
	}

	struct lval *result = builtin(value, first->sym);

	lval_del(first);
	return result;
}

struct lval *builtin(struct lval *value, char *sym)
{
	if (stringcmp("head", sym) == 0)
		return builtin_head(value);

	if (stringcmp("tail", sym) == 0)
		return builtin_tail(value);

	if (stringcmp("list", sym) == 0)
		return builtin_list(value);

	if (stringcmp("len", sym) == 0)
		return builtin_len(value);

	/*
	 * Example:
	 * eval (tail {tail head (list 69 42 69420)})
	 *
	 * The example above process is like this:
	 * - Evaluate the first `tail` keyword, which result in
	 *   `head (list 69 42 69420)`.
	 * - And then evaluate `head` keyword with argument
	 *   `(list 69 42 69420)`.
	 * - Finally, we evaluate the `list` keyword with
	 *   argument `69 42 69420`.
	 */
	if (stringcmp("eval", sym) == 0)
		return builtin_eval(value);

	/*
	 * Example:
	 * join {69} {69420 69} (eval {head (list 42 69420)})
	 *
	 * The example above process is like this:
	 * - We create new quoted expression, and put that
	 *   in some variable, let's say `joined` variable.
	 * - Add the element in {69} into `joined` quoted
	 *   expression.
	 * - And then, add all elements from {69420 69} into
	 *   `joined` quoted expression.
	 * - After that, we evaluate symbolic expression
	 *   (eval {head (list 42 69420)}), which result
	 *   in number 42 and put that in `joined` quoted
	 *   expression.
	 * - Finally, we got new `joined` quoted expression
	 *   with elements {69 69420 69 42}.
	 */
	if (stringcmp("join", sym) == 0)
		return builtin_join(value);

	if (strstr("+*-/%^", sym))
		return builtin_op(value, sym);

	lval_del(value);
	return lval_err(
		"unknown symbol",
		__FILE__,
		__LINE__
	);
}

struct lval *builtin_op(struct lval *value, char *op)
{
	int i;

	for (i = 0; i < value->count; i++) {
		struct lval *cell = value->cell[i];

		if (cell->type != LVAL_NUM) {
			lval_del(value);
			return lval_err(
				"cannot operate on non-number",
				__FILE__,
				__LINE__
			);
		}
	}

	struct lval *first = lval_pop(
		value,
		0,
		__FILE__,
		__LINE__
	);

	if (first->type == LVAL_ERR)
		return first;

	if ((*op == '-') && value->count == 0)
		first->num = -first->num;

	while (value->count > 0) {
		struct lval *next = lval_pop(
			value,
			0,
			__FILE__,
			__LINE__
		);

		if (next->type == LVAL_ERR) {
			lval_del(first);
			return next;
		}

		if (*op == '+')
			first->num += next->num;

		if (*op == '-')
			first->num -= next->num;

		if (*op == '*')
			first->num *= next->num;

		if (*op == '/') {
			if (next->num == 0) {
				lval_del(first);
				lval_del(next);

				first = lval_err(
					"division by zero",
					__FILE__,
					__LINE__
				);
				break;
			}

			first->num /= next->num;
		}

		if (*op == '%') {
			double modulo = fmod(first->num, next->num);

			if (isnan(modulo)) {
				first = lval_err(
					"invalid modulo operation",
					__FILE__,
					__LINE__
				);
				break;
			}

			first->num = modulo;
		}

		if (*op == '^') {
			double power = pow(first->num, next->num);

			if (isnan(power)) {
				first = lval_err(
					"invalid power operation",
					__FILE__,
					__LINE__
				);
				break;
			}

			first->num = power;
		}

		lval_del(next);
	}

	lval_del(value);
	return first;
}

struct lval *builtin_head(struct lval *value)
{
	if (value->count != 1) {
		lval_del(value);
		return lval_err(
			"builtin_head() passed too many arguments",
			__FILE__,
			__LINE__
		);
	}

	if (value->cell[0]->type != LVAL_QEXPR) {
		lval_del(value);
		return lval_err(
			"builtin_head() passed incorrect type",
			__FILE__,
			__LINE__
		);
	}

	if (value->cell[0]->count == 0) {
		lval_del(value);
		return lval_err(
			"builtin_head() passed empty list",
			__FILE__,
			__LINE__
		);
	}

	struct lval *head = lval_take(value, 0);

	while (head->count > 1) {
		struct lval *rest = lval_pop(
			head,
			1,
			__FILE__,
			__LINE__
		);

		if (rest->type == LVAL_ERR)
			return rest;

		lval_del(rest);
	}

	return head;
}

struct lval *builtin_tail(struct lval *value)
{
	if (value->count != 1) {
		lval_del(value);
		return lval_err(
			"builtin_tail() passed too many arguments",
			__FILE__,
			__LINE__
		);
	}

	if (value->cell[0]->type != LVAL_QEXPR) {
		lval_del(value);
		return lval_err(
			"builtin_tail() passed incorrect type",
			__FILE__,
			__LINE__
		);
	}

	if (value->cell[0]->count == 0) {
		lval_del(value);
		return lval_err(
			"builtin_tail() passed empty list",
			__FILE__,
			__LINE__
		);
	}

	struct lval *tail = lval_take(value, 0);

	struct lval *rest = lval_pop(
		tail,
		0,
		__FILE__,
		__LINE__
	);

	if (rest->type == LVAL_ERR)
		return rest;

	lval_del(rest);

	return tail;
}

struct lval *builtin_list(struct lval *value)
{
	value->type = LVAL_QEXPR;
	return value;
}

struct lval *builtin_eval(struct lval *value)
{
	if (value->count != 1) {
		lval_del(value);
		return lval_err(
			"builtin_eval() passed too many arguments",
			__FILE__,
			__LINE__
		);
	}

	if (value->cell[0]->type != LVAL_QEXPR) {
		lval_del(value);
		return lval_err(
			"builtin_eval() passed incorrect type",
			__FILE__,
			__LINE__
		);
	}

	struct lval *first = lval_take(value, 0);
	first->type = LVAL_SEXPR;

	return lval_eval(first);
}

struct lval *builtin_join(struct lval *value)
{
	int i;

	for (i = 0; i < value->count; i++) {
		if (value->cell[i]->type != LVAL_QEXPR) {
			lval_del(value);
			return lval_err(
				"builtin_join() passed incorrect type",
				__FILE__,
				__LINE__
			);
		}
	}

	struct lval *joined = lval_pop(
		value,
		0,
		__FILE__,
		__LINE__
	);

	if (joined->type == LVAL_ERR)
		return joined;

	while (value->count) {
		struct lval *next = lval_pop(
			value,
			0,
			__FILE__,
			__LINE__
		);

		if (next->type == LVAL_ERR) {
			lval_del(joined);
			return next;
		}

		struct lval *new_joined = lval_join(
			joined,
			next
		);

		if (new_joined->type == LVAL_ERR)
			return new_joined;

		joined = new_joined;
		new_joined = NULL;
	}

	lval_del(value);
	return joined;
}

/*
 * Usage:
 * len {69 69 69420} -> 3
 * len {69 42 (head {42 69}) (list 69 42 69420)} -> 4
 *
 * Keep in mind that we don't evaluate anything inside
 * quoted expression. So if we provide the
 * (head {42 69}), it will be treated as 1 item.
 *
 * Reference:
 * https://jtra.cz/stuff/lisp/sclr/length.html
 */
struct lval *builtin_len(struct lval *value)
{
	if (value->count != 1) {
		lval_del(value);
		return lval_err(
			"builtin_len() passed too many arguments",
			__FILE__,
			__LINE__
		);
	}

	if (value->cell[0]->type != LVAL_QEXPR) {
		lval_del(value);
		return lval_err(
			"builtin_len() passed incorrect type",
			__FILE__,
			__LINE__
		);
	}

	value->type = LVAL_QEXPR_LEN;

	if (value->cell[0]->count == 0)
		value->count = 0;
	else
		value->count = value->cell[0]->count;

	return value;
}

struct lval *lval_join(struct lval *v1, struct lval *v2)
{
	/*
	 * Move the value of v2 into v1 and reduce the
	 * v2 items until v2 is empty.
	 */
	while (v2->count) {
		struct lval *element = lval_pop(
			v2,
			0,
			__FILE__,
			__LINE__
		);

		if (element->type == LVAL_ERR) {
			lval_del(v1);
			return element;
		}

		v1 = lval_add(
			v1,
			element
		);
	}

	lval_del(v2);
	return v1;
}

/*
 * lval_take() to get the element from list
 * and then free the memory of original list.
 */
struct lval *lval_take(struct lval *value, int i)
{
	struct lval *item = lval_pop(
		value,
		i,
		__FILE__,
		__LINE__
	);

	if (item->type == LVAL_ERR)
		return item;

	lval_del(value);
	return item;
}

/*
 * lval_pop() to extract a single element from list, which
 * means that we are reducing the list's elements,
 * and then return the extracted value.
 */
struct lval *lval_pop(
	struct lval *value,
	int i,
	const char *filename,
	unsigned int line_number
)
{
	if (value->count <= i) {
		lval_del(value);
		return lval_err(
			"lval_pop index out of bounds",
			filename,
			line_number
		);
	}

	struct lval *item = value->cell[i];

	/*
	 * We can safely provide the (value-count - 1) or
	 * the max value in 0-index based as the index
	 * because when the size is 0, memmove() will move 0
	 * bytes and that means memmove() will not move
	 * any data.
	 *
	 * Reference:
	 * https://stackoverflow.com/a/3751937
	 */
	memmove(
		value->cell + i,
		value->cell + (i + 1),
		sizeof(*value->cell) * (value->count - i - 1)
	);

	value->count--;

	struct lval **new_alloc = alloc_util(
		value->cell,
		value->count,
		sizeof(*value->cell),
		__FILE__,
		__LINE__
	);

	if (
		new_alloc == NULL &&
		(sizeof(*value->cell) * value->count) > 0
	)
		alloc_err(
			REALLOC_ERR_MSG,
			__FILE__,
			__LINE__
		);

	value->cell = new_alloc;
	new_alloc = NULL;

	return item;
}

void lval_println(struct lval *value)
{
	lval_print(value);
	putchar('\n');
}

static void lval_expr_print(
	struct lval *v,
	char open,
	char close
)
{
	int i;

	if (v->count == 0) {
		printf("()");
		return;
	}

	putchar(open);

	for (i = 0; i < v->count; i++) {
		lval_print(v->cell[i]);

		if (i != (v->count - 1))
			putchar(' ');
	}

	putchar(close);
}

static void lval_print(struct lval *value)
{
	switch (value->type) {
		case LVAL_NUM:
			printf("%.3lf", value->num);
			break;

		case LVAL_ERR:
			printf("Error: %s", value->err);
			break;

		case LVAL_SYM:
			printf("%s", value->sym);
			break;

		case LVAL_SEXPR:
		case LVAL_QEXPR:
			lval_expr_print(value, '(', ')');
			break;

		case LVAL_QEXPR_LEN:
			printf(
				"Total q-expr: %d",
				value->count
			);
			break;

		default:
			printf("Unknown value type.\n");
	}
}

struct lval *lval_add(struct lval *v1, struct lval *v2)
{
	v1->count++;

	struct lval **new_alloc = alloc_util(
		v1->cell,
		v1->count,
		sizeof(*v1->cell),
		__FILE__,
		__LINE__
	);

	if (
		new_alloc == NULL &&
		(sizeof(*v1->cell) * v1->count) > 0
	)
		alloc_err(
			REALLOC_ERR_MSG,
			__FILE__,
			__LINE__
		);

	v1->cell = new_alloc;
	new_alloc = NULL;

	v1->cell[v1->count - 1] = v2;
	return v1;
}

void lval_del(struct lval *value)
{
	int i;

	switch (value->type) {
		case LVAL_NUM:
		case LVAL_SYM:
			break;

		case LVAL_ERR:
			free(value->err);
			break;

		case LVAL_QEXPR_LEN:
			for (i = 0; i < value->cell[0]->count; i++)
				free(value->cell[0]->cell[i]);

			break;

		case LVAL_SEXPR:
		case LVAL_QEXPR:
			for (i = 0; i < value->count; i++)
				lval_del(value->cell[i]);

			free(value->cell);
			value->cell = NULL;
			break;
	}

	free(value);
}

struct lval *lval_err(
	const char *mes,
	const char *filename,
	unsigned int line_number
)
{
	unsigned int limit;
	const char *fmt;

	struct lval *value = malloc(sizeof(*value));

	if (value == NULL)
		alloc_err(
			MALLOC_ERR_MSG,
			__FILE__,
			__LINE__
		);

	value->type = LVAL_ERR;

	/*
	 * We expected to use gcc as the compiler and gcc has
	 * __builtin_uadd_overflow(), so if you use another
	 * compiler, make sure that the compiler has
	 * __builtin_uadd_overflow().
	 */
	unsigned int len_mes, overflow_indicator;

	overflow_indicator = __builtin_uadd_overflow(
		strlen(mes),
		1,
		&len_mes
	);

	if (overflow_indicator)
		int_overflow_err(__FILE__, __LINE__);


#ifdef DEBUG
	/*
	 * temp_len to store the length filename and line number.
	 */
	unsigned int len_filename, temp_len;

	/*
	 * strlen() exclude the null terminator (\0) and
	 * because we already add the null terminator
	 * character in len_mes, we don't need to add another
	 * null terminator in len_filename.
	 */
	len_filename = strlen(filename);

	/*
	 * floor(log10(...)) result is 1 character less than
	 * the actual line number characters. For example,
	 * floor(log10(69)) will result in 1 rather than 2.
	 *
	 * Reference:
	 * https://www.dhairyashah.dev/posts/how-to-get-the-length-of-a-number-in-c/
	 */
	unsigned int len_line_number = floor(
		log10(line_number)
	) + 1;

	overflow_indicator = __builtin_uadd_overflow(
		len_filename,
		len_line_number,
		&temp_len
	);

	if (overflow_indicator)
		int_overflow_err(__FILE__, __LINE__);

	/*
	 * Result:
	 * mes (utils.c:69)\0
	 *
	 * __builtin_addc() is a builtin function from gcc.
	 * So if you use another compiler, make sure your
	 * compiler has __builtin_addc().
	 *
	 * Reference:
	 * https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html
	 */
	limit = __builtin_addc(
		len_mes,
		temp_len,
		4,
		&overflow_indicator
	);

	if (overflow_indicator)
		int_overflow_err(__FILE__, __LINE__);

	fmt = "%s (%s:%u)";
#else
	limit = len_mes;
	fmt = "%s";
#endif

	/*
	 * We don't need to use malloc() and strcpy()
	 * if we use string literal, which has
	 * static storage duration and __persist__ the
	 * entire execution of the program. So string
	 * literal does not result in __dangling
	 * pointer__.
	 *
	 * With that in mind, we can safely change the
	 * memory address stored in value->err with
	 * string literal memory address.
	 *
	 * malloc() and strcpy() needed if we get
	 * the value during run-time and the memory
	 * will be freed in the middle of processing.
	 * For example having string from opening a
	 * file, at some point we need to close the
	 * file and the memory address will no longer
	 * accessible.
	 *
	 * References:
	 * - https://stackoverflow.com/a/9970305
	 * - https://en.cppreference.com/w/c/language/string_literal.html
	 * - https://stackoverflow.com/a/55931977
	 * - https://stackoverflow.com/a/55723074
	 */
	value->err = malloc(limit);

	if (value->err == NULL)
		alloc_err(
			MALLOC_ERR_MSG,
			__FILE__,
			__LINE__
		);

	snprintf(
		value->err,
		limit,
		fmt,
		mes,
		filename,
		line_number
	);

	return value;
}

struct lval *lval_num(double num)
{
	struct lval *value = malloc(sizeof(*value));

	if (value == NULL)
		alloc_err(
			MALLOC_ERR_MSG,
			__FILE__,
			__LINE__
		);

	value->type = LVAL_NUM;
	value->num = num;

	return value;
}

struct lval *lval_sym(char *sym)
{
	struct lval *value = malloc(sizeof(*value));

	if (value == NULL)
		alloc_err(
			MALLOC_ERR_MSG,
			__FILE__,
			__LINE__
		);

	value->type = LVAL_SYM;

	/*
	 * We didn't use malloc() and strcpy() here
	 * because the memory address of `sym` (character
	 * for the symbol, such as +, -, *, and so on)
	 * will be available during the evaluation until
	 * we call mpc_ast_delete() in the main().
	 *
	 * Reference:
	 * https://stackoverflow.com/a/55723683
	 */
	value->sym = sym;

	return value;
}

struct lval *lval_sexpr(void)
{
	struct lval *value = malloc(sizeof(*value));

	if (value == NULL)
		alloc_err(
			MALLOC_ERR_MSG,
			__FILE__,
			__LINE__
		);

	value->type = LVAL_SEXPR;
	value->count = 0;
	value->cell = NULL;

	return value;
}

struct lval *lval_qexpr(void)
{
	struct lval *value = malloc(sizeof(*value));

	if (value == NULL)
		alloc_err(
			MALLOC_ERR_MSG,
			__FILE__,
			__LINE__
		);

	value->type = LVAL_QEXPR;
	value->count = 0;
	value->cell = NULL;

	return value;
}

/*
 * References:
 * - https://stackoverflow.com/a/67520047
 * - https://www.reddit.com/r/learnprogramming/comments/7xzaiv/comment/ducbt4y
 *
 * Strings literals are pointers in C.
 * When you see a sentence like "Hello world!",
 * that variable is of the type const char*,
 * meaning it is a pointer to a bunch of chars that cannot be modified.
 */
int stringcmp(const char *str1, const char *str2)
{
	while (*str1 && *str1 == *str2) {
		str1++;
		str2++;
	}

	int result = (unsigned char)*str1 - (unsigned char)*str2;
	return result;
}

/*
 * Why using function instead of macro reference:
 * https://stackoverflow.com/a/9104594
 */
void int_overflow_err(
	const char *filename,
	unsigned int line_number
)
{
	printf(
		"Error: integer overflow on %s:%u\n",
		filename,
		line_number
	);
	exit(42);
}

void alloc_err(
	const char *msg,
	const char *filename,
	unsigned int line_number
)
{
	/*
	 * printf() is using stdout, which is a static buffer
	 * that are pre-allocated during program startup. So
	 * even though memory allocation failed, we can safely
	 * use printf() to print out the error message.
	 *
	 * References:
	 * - https://linuxvox.com/blog/which-c-standard-library-functions-use-malloc-under-the-hood/
	 * - https://www.gnu.org/software/libc/manual/html_node/Standard-Streams.html
	 * - https://stackoverflow.com/a/16430155
	 * - https://stackoverflow.com/a/1244413
	 */
	printf("%s (%s:%u)\n", msg, filename, line_number);
	exit(69);
}

/*
 * It seems like when using realloc() with the NULL value for
 * provided memory block, realloc() become equivalent to
 * malloc().
 *
 * For example:
 * realloc(NULL, sizeof(int) * 2)
 * equivalent to
 * malloc(sizeof(int) * 2)
 *
 * References:
 * - https://lteo.net/blog/2014/10/28/reallocarray-in-openbsd-integer-overflow-detection-for-free/
 * - http://cvsweb.openbsd.org/cgi-bin/cvsweb/~checkout~/src/lib/libc/stdlib/reallocarray.c
 * - https://stackoverflow.com/a/2849850 (__FILE__ and __LINE__ preprocessor macro)
 */
void *alloc_util(
	void *ptr,
	size_t total_elements,
	size_t size,
	const char *filename,
	unsigned int line_number
)
{
	/*
	 * We PTRDIFF_MAX instead of SIZE_MAX because when
	 * we try to allocate more than PTRDIFF_MAX bytes,
	 * realloc() will consider that an error and will
	 * return NULL.
	 *
	 * So let's try preventing that error too in the
	 * multiplication overflow checker and see how it
	 * goes.
	 */
	if (
		(
		 total_elements >= OVERFLOW_VALUE(total_elements) ||
		 size >= OVERFLOW_VALUE(size)
		) ||
		(
		 total_elements > 0 &&
		 (PTRDIFF_MAX / total_elements) < size
		)
	)
		int_overflow_err(filename, line_number);


	return realloc(ptr, total_elements * size);
}
