#include "mpc.h"
#include "stringcmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <editline/readline.h>

enum lval_type {
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
	char *err;
	char *sym;
	int count;
	struct lval **cell;
};

struct lval *lval_read(mpc_ast_t *ast);
void lval_println(struct lval *value);
void lval_expr_print(struct lval *v, char open, char close);
void lval_print(struct lval *value);
struct lval *lval_add(struct lval *v1, struct lval *v2);
void lval_del(struct lval *value);
struct lval *lval_err(char *mes);
struct lval *lval_read_num(mpc_ast_t *ast);
struct lval *lval_num(double num);
struct lval *lval_sym(char *sym);
struct lval *lval_sexpr(void);

int main(int argc, char **argv)
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
			struct lval *value = lval_read(ast);
			lval_println(value);
			lval_del(value);

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

struct lval *lval_read(mpc_ast_t *ast)
{
	int i;
	struct lval *num = NULL;

	if (strstr(ast->tag, "number"))
		return lval_read_num(ast);

	if (strstr(ast->tag, "symbol"))
		return lval_sym(ast->contents);

	/*
	 * Symbol > is the root for the parser (?).
	 */
	if (stringcmp(ast->tag, ">") == 0
		|| strstr(ast->tag, "sexpr"))
		num = lval_sexpr();

	for (i = 0; i < ast->children_num; i++) {
		if (stringcmp(ast->children[i]->contents, "(") == 0
			|| stringcmp(ast->children[i]->contents, ")") == 0
			|| stringcmp(ast->children[i]->tag, "regex") == 0)
			continue;

		num = lval_add(
			num,
			lval_read(ast->children[i])
		);
	}

	return num;
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
		case LVAL_NUM:
			break;

		case LVAL_ERR:
			free(value->err);
			break;

		case LVAL_SYM:
			free(value->sym);
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

struct lval *lval_err(char *mes)
{
	struct lval *value = malloc(sizeof(struct lval));

	value->type = LVAL_ERR;
	value->err = malloc(strlen(mes) + 1);
	strcpy(value->err, mes);

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
	struct lval *value = malloc(sizeof(struct lval));

	value->type = LVAL_NUM;
	value->num = num;

	return value;
}

struct lval *lval_sym(char *sym)
{
	struct lval *value = malloc(sizeof(struct lval));

	value->type = LVAL_SYM;
	value->sym = malloc(strlen(sym) + 1);
	strcpy(value->sym, sym);

	return value;
}

struct lval *lval_sexpr(void)
{
	struct lval *value = malloc(sizeof(struct lval));

	value->type = LVAL_SEXPR;
	value->count = 0;
	value->cell = NULL;

	return value;
}
