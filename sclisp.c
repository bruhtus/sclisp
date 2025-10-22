#include "mpc.h"
#include "stringcmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <editline/readline.h>

enum lval_type {
	LVAL_ERR = -1,
	LVAL_NUM = 1
};

enum lval_err {
	LERR_INVALID_OP = 0,
	LERR_INVALID_NUM = 1,
	LERR_DIV_ZERO = 2,
	LERR_MODULO_NAN = 3
};

/* Lisp value. */
struct lval {
	enum lval_type type;
	double num;
	enum lval_err err;
};

struct lval eval(mpc_ast_t *ast);
struct lval eval_op(struct lval x, char *op, struct lval y);
void lval_print(struct lval value);
struct lval lval_num(double x);
struct lval lval_err(enum lval_err x);

int main(int argc, char **argv)
{
	mpc_parser_t *Number = mpc_new("number");
	mpc_parser_t *Operator = mpc_new("operator");
	mpc_parser_t *Expr = mpc_new("expr");
	mpc_parser_t *Sclisp = mpc_new("sclisp");

	mpc_parser_t *init_parsers[] = {
		Number,
		Operator,
		Expr,
		Sclisp
	};

	int parsers_len = sizeof(init_parsers) / sizeof(init_parsers[0]);

	mpca_lang(
		MPCA_LANG_DEFAULT,
		"number: /-?\\d+(\\.\\d+)?/;"
		"operator: '+' | '-' | '*' | '/' | '%';"
		"expr: <number> | '(' <operator> <expr>+ ')';"
		"sclisp: /^/ <operator>? <expr>+ /$/;",
		parsers_len, init_parsers
	);

	puts("Sclisp v0.0.1-rc1");
	puts("Press ctrl-c, ctrl-d, exit command to exit\n");

	char prompt[128] = "";

	while (1) {
		sprintf(prompt, "sclisp> ");
		char *input = readline(prompt);

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
			struct lval value = eval(ast);
			lval_print(value);

			mpc_ast_delete(result.output);
		} else {
			mpc_err_print(result.error);
			mpc_err_delete(result.error);
		}

		free(input);
	}

	mpc_cleanup(4, Number, Operator, Expr, Sclisp);
	return 0;
}

struct lval eval(mpc_ast_t *ast)
{
	char *substr = strstr(ast->tag, "number");

	if (substr) {
		/*
		 * Reference for errno: https://stackoverflow.com/a/46014661
		 */
		double num = strtod(ast->contents, NULL);
		return num == 0 && errno == ERANGE
			? lval_err(LERR_INVALID_NUM)
			: lval_num(num);
	}

	mpc_ast_t  **children = ast->children;
	int children_num = ast->children_num;

	struct lval x;

	/*
	 * Because we made the intial operator optional, we don't need to provide operator for the first number.
	 * This break the calculation because the expected minimum children_num decreased.
	 *
	 * Currently still not sure how the children_num calculated, but from the input of only number (without preceding operator), the result of children_num is 3. So when the children_num equal 3, we can just return the value directly.
	 */
	if (children_num == 3) {
		double num = strtod(children[1]->contents, NULL);
		return num == 0 && errno == ERANGE
			? lval_err(LERR_INVALID_NUM)
			: lval_num(num);
	}

	if (children_num >= 1) {
		char *op = children[1]->contents;
		x = eval(children[2]);

		int i = 3;
		while (strstr(children[i]->tag, "expr")) {
			x = eval_op(x, op, eval(children[i]));
			i++;
		}
	}

	return x;
}

struct lval eval_op(struct lval x, char *op, struct lval y)
{
	if (x.type == LVAL_ERR)
		return x;

	if (y.type == LVAL_ERR)
		return y;

	if (stringcmp(op, "+") == 0)
		return lval_num(x.num + y.num);

	if (stringcmp(op, "-") == 0)
		return lval_num(x.num - y.num);

	if (stringcmp(op, "*") == 0)
		return lval_num(x.num * y.num);

	if (stringcmp(op, "/") == 0) {
		return y.num == 0
			? lval_err(LERR_DIV_ZERO)
			: lval_num(x.num / y.num);
	}

	if (stringcmp(op, "%") == 0) {
		double modulo = fmod(x.num, y.num);

		return isnan(modulo)
			? lval_err(LERR_MODULO_NAN)
			: lval_num(modulo);
	}

	return lval_err(LERR_INVALID_OP);

}

void lval_print(struct lval value)
{
	const char *err;

	switch (value.type) {
		case LVAL_NUM:
			printf("%.3lf\n", value.num);
			break;

		case LVAL_ERR:
			if (value.err == LERR_INVALID_OP)
				err = "Error: invalid operator";
			else if (value.err == LERR_INVALID_NUM)
				err = "Error: invalid number";
			else if (value.err == LERR_DIV_ZERO)
				err = "Error: division by zero";
			else if (value.err == LERR_MODULO_NAN)
				err = "Error: invalid modulo operation";
			else
				err = "Error: unrecognized error";

			printf("%s\n", err);
			break;

		default:
			printf("Unknown value type.\n");
	}
}

struct lval lval_num(double x)
{
	struct lval value = {
		.type = LVAL_NUM,
		.num = x
	};
	return value;
}

struct lval lval_err(enum lval_err x)
{
	struct lval value = {
		.type = LVAL_ERR,
		.err = x
	};
	return value;
}
