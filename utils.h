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

enum ltype_e {
	LVAL_ERR = -1,
	LVAL_NUM = 1,
	LVAL_SYM = 2,
	LVAL_SEXPR = 3,
	LVAL_QEXPR = 4,
	LVAL_QEXPR_LEN = 5,
	LVAL_FUNC = 6
};

struct lval;
struct lenv;

typedef struct lval *(*lbuiltin_td)(
	struct lenv *,
	struct lval *
);

/*
 * Lisp value.
 *
 * The limit of double value is DBL_MAX from float.h.
 *
 * To reproduce the double overflow, we can do:
 * DBL_MAX + 1E+292
 *
 * We use unsigned int for count because count expected
 * not to have negative value.
 *
 * Reference:
 * https://stackoverflow.com/a/37830627
 */
struct lval {
	enum ltype_e type;

	double num;
	char *err;
	char *sym;

	unsigned int count;
	struct lval **cell;

	lbuiltin_td builtin;
	const char *builtin_name;
};

struct lenv {
	unsigned int count;
	char **syms;
	struct lval **vals;
};

struct fbuiltin {
	lbuiltin_td func;
	const char *name;
};

struct lval *lval_eval(
	struct lenv *env,
	struct lval *value
);

struct lval *lval_eval_sexpr(
	struct lenv *env,
	struct lval *value
);

struct lval *lval_copy(struct lval *value);

struct lval *builtin_arith(
	struct lval *value,
	char op
);

struct lval *builtin_let(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_head(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_tail(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_list(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_eval(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_join(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_len(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_add(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_sub(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_mul(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_div(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_mod(
	struct lenv *env,
	struct lval *value
);

struct lval *builtin_pow(
	struct lenv *env,
	struct lval *value
);

struct lval *lval_join(struct lval *v1, struct lval *v2);
struct lval *lval_take(struct lval *value, int i);

/*
 * We are using unsigned int for i because in this case,
 * index should not be negative value.
 */
struct lval *lval_pop(
	struct lval *value,
	unsigned int i,
	const char *filename,
	unsigned int line_number
);

void lval_println(struct lval *value);

struct lval *lval_add(struct lval *v1, struct lval *v2);
void lval_del(struct lval *value);

struct lval *lval_err(
	const char *mes,
	const char *filename,
	unsigned int line_number
);

struct lval *lval_builtin(
	lbuiltin_td func,
	const char *func_name
);

struct lval *lval_num(double num);
struct lval *lval_sym(char *sym);
struct lval *lval_sexpr(void);
struct lval *lval_qexpr(void);

struct lenv *lenv_init(void);
void lenv_del(struct lenv *env);

void lenv_add_builtin(
	struct lenv *env,
	char *name,
	lbuiltin_td func,
	const char *func_name
);

void lenv_builtins_init(struct lenv *env);

struct lval *lenv_get(
	struct lenv *env,
	struct lval *value
);

void lenv_put(
	struct lenv *env,
	char *sym,
	struct lval *value
);

int stringcmp(const char *str1, const char *str2);

void stringcpy(
	char *dest,
	size_t dest_len,
	char *src,
	size_t src_len,
	const char *filename,
	unsigned int line_number
);

void int_overflow_err(
	const char *filename,
	unsigned int line_number
);

void alloc_err(
	const char *msg,
	const char *filename,
	unsigned int line_number
);

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
