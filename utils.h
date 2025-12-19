#include "mpc.h"

#define MAX_BIT_SIZE(type) ( \
	(sizeof(type) * CHAR_BIT) - 1 \
)

enum lval_type {
	LVAL_MALLOC_FAILED = -2,
	LVAL_ERR = -1,
	LVAL_NUM = 1,
	LVAL_SYM = 2,
	LVAL_SEXPR = 3,
	LVAL_QEXPR = 4
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

struct lval *lval_join(struct lval *v1, struct lval *v2);
struct lval *lval_take(struct lval *value, int i);
struct lval *lval_pop(struct lval *value, int i);
struct lval *lval_read(mpc_ast_t *ast);

void lval_println(struct lval *value);
void lval_expr_print(struct lval *v, char open, char close);
void lval_print(struct lval *value);

struct lval *lval_add(struct lval *v1, struct lval *v2);
void lval_del(struct lval *value);

struct lval *lval_err(const char *mes);
struct lval *lval_read_num(mpc_ast_t *ast);
struct lval *lval_num(double num);
struct lval *lval_sym(char *sym);
struct lval *lval_sexpr(void);
struct lval *lval_qexpr(void);

struct lval *malloc_err(void);

int stringcmp(const char *str1, const char *str2);
