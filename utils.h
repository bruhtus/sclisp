/*
 * Only include header file once.
 *
 * Reference about header file:
 * - https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Once_002dOnly-Headers.html
 * - https://stackoverflow.com/q/232785 (#pragma directive).
 * - https://stackoverflow.com/a/1804719 (About include in header file).
 */
#ifndef SCLISP_UTILS
#define SCLISP_UTILS

#include <stddef.h>

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

#define MALLOC_ERR_MSG "malloc failed\n"
#define SIZE_MALLOC_ERR_MSG sizeof(MALLOC_ERR_MSG)

#define REALLOC_ERR_MSG "realloc failed\n"
#define SIZE_REALLOC_ERR_MSG sizeof(REALLOC_ERR_MSG)

enum lval_type {
	LVAL_ERR = -1,
	LVAL_NUM = 1,
	LVAL_SYM = 2,
	LVAL_SEXPR = 3,
	LVAL_QEXPR = 4,
	LVAL_QEXPR_LEN = 5
};

enum lval_err {
	LERR_INVALID_SYM = 0,
	LERR_INVALID_NUM = 1,
	LERR_DIV_ZERO = 2,
	LERR_MODULO_NAN = 3,
	LERR_POW_NAN = 4
};

/*
 * Lisp value.
 *
 * The limit of double value is DBL_MAX from float.h.
 *
 * To reproduce the double overflow, we can do:
 * DBL_MAX + 1E+292
 *
 * Reference:
 * https://stackoverflow.com/a/37830627
 */
struct lval {
	enum lval_type type;
	double num;
	const char *err;
	char *sym;
	int count;
	struct lval **cell;
};

struct lval *lval_eval(struct lval *value);
struct lval *lval_eval_sexpr(struct lval *value);

struct lval *builtin(struct lval *value, char *sym);
struct lval *builtin_op(struct lval *value, char *op);

struct lval *builtin_head(struct lval *value);
struct lval *builtin_tail(struct lval *value);
struct lval *builtin_list(struct lval *value);
struct lval *builtin_eval(struct lval *value);
struct lval *builtin_join(struct lval *value);
struct lval *builtin_len(struct lval *value);

struct lval *lval_join(struct lval *v1, struct lval *v2);
struct lval *lval_take(struct lval *value, int i);
struct lval *lval_pop(struct lval *value, int i);

void lval_println(struct lval *value);
void lval_expr_print(struct lval *v, char open, char close);
void lval_print(struct lval *value);

struct lval *lval_add(struct lval *v1, struct lval *v2);
void lval_del(struct lval *value);

struct lval *lval_err(const char *mes);
struct lval *lval_num(double num);
struct lval *lval_sym(char *sym);
struct lval *lval_sexpr(void);
struct lval *lval_qexpr(void);

int stringcmp(const char *str1, const char *str2);

/*
 * Similar to reallocarray() but using our internal error
 * handling.
 */
void *alloc_util(
	void *ptr,
	size_t total_elements,
	size_t size,
	const char *filename,
	unsigned int line_number
);

#endif /* sclisp utils included */
