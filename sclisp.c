#include "mpc.h"
#include "stringcmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <editline/readline.h>

enum lval_type {
	LVAL_MALLOC_FAILED = -2,
	LVAL_ERR = -1,
	LVAL_NUM = 1,
	LVAL_SYM = 2,
	LVAL_SEXPR = 3
};

enum lval_err {
	LERR_INVALID_SYM = 0,
	LERR_INVALID_NUM = 1,
	LERR_DIV_ZERO = 2,
	LERR_MODULO_NAN = 3,
	LERR_POW_NAN = 4
};

/* Lisp value. */
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
struct lval *builtin_op(struct lval *value, char *op);
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
struct lval *malloc_err(void);

int main(void)
{
	mpc_parser_t *Number = mpc_new("number");
	mpc_parser_t *Symbol = mpc_new("symbol");
	mpc_parser_t *Sexpr = mpc_new("sexpr");
	mpc_parser_t *Expr = mpc_new("expr");
	mpc_parser_t *Sclisp = mpc_new("sclisp");

	mpc_parser_t *init_parsers[] = {
		Number,
		Symbol,
		Sexpr,
		Expr,
		Sclisp
	};

	int parsers_len = sizeof(init_parsers) / sizeof(init_parsers[0]);

	mpca_lang(
		MPCA_LANG_DEFAULT,
		"number: /-?\\d+(\\.\\d+)?/;"
		"symbol: '+' | '-' | '*' | '/' | '%' | '^';"
		"sexpr: '(' <expr>* ')';"
		"expr: <number> | <symbol> | <sexpr>;"
		"sclisp: /^/ <expr>* /$/;",
		parsers_len, init_parsers
	);

	puts("Sclisp v0.0.1-rc1");
	puts("Press ctrl-c, ctrl-d, exit command to exit\n");

	char prompt[128] = "";
	char *input;

	while (1) {
		sprintf(prompt, "sclisp> ");
		input = readline(prompt);

		if (input == NULL || stringcmp(input, "exit") == 0) {
			free(input);
			break;
		}

		add_history(input);

		mpc_result_t result;

		int parsed = mpc_parse(
			"<stdin>",
			input,
			Sclisp,
			&result
		);

		if (parsed) {
			mpc_ast_t *ast = result.output;

			struct lval *eval = lval_eval(
				lval_read(ast)
			);

			lval_println(eval);
			lval_del(eval);

			mpc_ast_delete(result.output);
		} else {
			mpc_err_print(result.error);
			mpc_err_delete(result.error);
		}

		free(input);
	}

	mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Sclisp);
	return 0;
}

struct lval *lval_eval(struct lval *value)
{
	if (value->type == LVAL_SEXPR)
		return lval_eval_sexpr(value);

	return value;
}

struct lval *lval_eval_sexpr(struct lval *value)
{
	int i;

	if (value->count == 0)
		return value;

	for (i = 0; i < value->count; i++) {
		value->cell[i] = lval_eval(value->cell[i]);

		if (value->cell[i]->type == LVAL_ERR)
			return lval_take(value, i);
	}

	if (value->count == 1)
		return lval_take(value, 0);

	struct lval *first = lval_pop(value, 0);

	if (first->type != LVAL_SYM) {
		lval_del(first);
		lval_del(value);
		return lval_err(
			"s-expression does not start with symbol"
		);
	}

	struct lval *result = builtin_op(value, first->sym);

	lval_del(first);
	return result;
}

struct lval *builtin_op(struct lval *value, char *op)
{
	int i;

	for (i = 0; i < value->count; i++) {
		if (value->cell[i]->type != LVAL_NUM) {
			lval_del(value);
			return lval_err(
				"cannot operate on non-number"
			);
		}
	}

	struct lval *first = lval_pop(value, 0);

	if ((stringcmp(op, "-") == 0) && value->count == 0) {
		first->num = -first->num;
	}

	while (value->count > 0) {
		struct lval *next = lval_pop(value, 0);

		if (stringcmp(op, "+") == 0)
			first->num += next->num;

		if (stringcmp(op, "-") == 0)
			first->num -= next->num;

		if (stringcmp(op, "*") == 0)
			first->num *= next->num;

		if (stringcmp(op, "/") == 0) {
			if (next->num == 0) {
				lval_del(first);
				lval_del(next);

				first = lval_err(
					"division by zero"
				);
				break;
			}

			first->num /= next->num;
		}

		if (stringcmp(op, "%") == 0) {
			double modulo = fmod(first->num, next->num);

			if (isnan(modulo)) {
				first = lval_err("invalid modulo operation");
				break;
			}

			first->num = modulo;
		}

		if (stringcmp(op, "^") == 0) {
			double power = pow(first->num, next->num);

			if (isnan(power)) {
				first = lval_err("invalid power operation");
				break;
			}

			first->num = power;
		}

		lval_del(next);
	}

	lval_del(value);
	return first;
}

struct lval *lval_take(struct lval *value, int i)
{
	struct lval *item = lval_pop(value, i);

	lval_del(value);
	return item;
}

struct lval *lval_pop(struct lval *value, int i)
{
	struct lval *item = value->cell[i];

	memmove(
		&value->cell[i],
		&value->cell[i+1],
		sizeof(struct lval *) * (value->count - i - 1)
	);

	value->count--;

	value->cell = realloc(
		value->cell,
		sizeof(struct lval *) * value->count
	);

	return item;
}

struct lval *lval_read(mpc_ast_t *ast)
{
	int i;
	struct lval *value = NULL;

	if (strstr(ast->tag, "number"))
		return lval_read_num(ast);

	if (strstr(ast->tag, "symbol"))
		return lval_sym(ast->contents);

	/*
	 * Symbol > is the root for the parser (?).
	 */
	if (stringcmp(ast->tag, ">") == 0
		|| strstr(ast->tag, "sexpr"))
		value = lval_sexpr();

	for (i = 0; i < ast->children_num; i++) {
		if (stringcmp(ast->children[i]->contents, "(") == 0
			|| stringcmp(ast->children[i]->contents, ")") == 0
			|| stringcmp(ast->children[i]->tag, "regex") == 0)
			continue;

		value = lval_add(
			value,
			lval_read(ast->children[i])
		);
	}

	return value;
}

void lval_println(struct lval *value)
{
	lval_print(value);
	putchar('\n');
}

void lval_expr_print(struct lval *v, char open, char close)
{
	int i;

	putchar(open);

	for (i = 0; i < v->count; i++) {
		lval_print(v->cell[i]);

		if (i != (v->count - 1)) {
			putchar(' ');
		}
	}

	putchar(close);
}

void lval_print(struct lval *value)
{
	switch (value->type) {
		case LVAL_NUM:
			printf("%.3lf", value->num);
			break;

		case LVAL_ERR:
		case LVAL_MALLOC_FAILED:
			printf("Error: %s", value->err);
			break;

		case LVAL_SYM:
			printf("%s", value->sym);
			break;

		case LVAL_SEXPR:
			lval_expr_print(value, '(', ')');
			break;

		default:
			printf("Unknown value type.\n");
	}
}

struct lval *lval_add(struct lval *v1, struct lval *v2)
{
	v1->count++;

	v1->cell = realloc(
		v1->cell,
		sizeof(struct lval *) * v1->count
	);

	v1->cell[v1->count - 1] = v2;
	return v1;
}

void lval_del(struct lval *value)
{
	int i;

	switch (value->type) {
		case LVAL_MALLOC_FAILED:
			return;

		case LVAL_NUM:
			break;

		case LVAL_ERR:
			break;

		case LVAL_SYM:
			break;

		case LVAL_SEXPR:
			for (i = 0; i < value->count; i++) {
				lval_del(value->cell[i]);
			}

			free(value->cell);
			value->cell = NULL;
			break;
	}

	free(value);
}

struct lval *lval_err(const char *mes)
{
	struct lval *value = malloc(sizeof(*value));

	if (value == NULL) {
		return malloc_err();
	}

	value->type = LVAL_ERR;

	/*
	 * We can directly use the string literal because
	 * we know the value before run-time (we give the
	 * value in the source code), so we don't
	 * need to use malloc() and strcpy().
	 *
	 * References:
	 * - https://stackoverflow.com/a/55931977
	 * - https://stackoverflow.com/a/55723074
	 */
	value->err = mes;

	return value;
}

struct lval *lval_read_num(mpc_ast_t *ast)
{
	double num = strtod(ast->contents, NULL);

	return num == 0 && errno == ERANGE
		? lval_err("invalid number")
		: lval_num(num);
}

struct lval *lval_num(double num)
{
	struct lval *value = malloc(sizeof(*value));

	if (value == NULL) {
		return malloc_err();
	}

	value->type = LVAL_NUM;
	value->num = num;

	return value;
}

struct lval *lval_sym(char *sym)
{
	struct lval *value = malloc(sizeof(*value));

	if (value == NULL) {
		return malloc_err();
	}

	value->type = LVAL_SYM;
	value->sym = sym;

	return value;
}

struct lval *lval_sexpr(void)
{
	struct lval *value = malloc(sizeof(*value));

	if (value == NULL) {
		return malloc_err();
	}

	value->type = LVAL_SEXPR;
	value->count = 0;
	value->cell = NULL;

	return value;
}

struct lval *malloc_err(void)
{
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
	static struct lval malloc_err;
	malloc_err.type = LVAL_MALLOC_FAILED;
	malloc_err.err = "malloc failed";

	return &malloc_err;
}
