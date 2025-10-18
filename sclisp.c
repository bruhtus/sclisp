#include "mpc.h"
#include "stringcmp.h"

#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>

long eval(mpc_ast_t *ast);
long eval_op(long x, char *op, long y);

int main()
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
		"number: /-?\\d+([\\.,]\\d+)?/;"
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
			long node = eval(ast);
			printf("%d\n", node);

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

long eval(mpc_ast_t *ast)
{
	char *substr = strstr(ast->tag, "number");

	if (substr)
		return atoi(ast->contents);

	char *op = NULL;
	long x = 0;

	mpc_ast_t  **children = ast->children;
	int children_num = ast->children_num;

	/*
	 * Because we made the intial operator optional, we don't need to provide operator for the first number.
	 * This break the calculation because the expected minimum children_num decreased.
	 *
	 * Currently still not sure how the children_num calculated, but from the input of only number (without preceding operator), the result of children_num is 3. So when the children_num equal 3, we can just return the value directly.
	 */
	if (children_num == 3) {
		x = atoi(children[1]->contents);
		return x;
	}

	if (children_num >= 1) {
		op = children[1]->contents;
		x = eval(children[2]);

		int i = 3;
		while (strstr(children[i]->tag, "expr")) {
			x = eval_op(x, op, eval(children[i]));
			i++;
		}
	}

	return x;
}

long eval_op(long x, char *op, long y)
{
	if (op == NULL)
		return 0;

	if (stringcmp(op, "+") == 0)
		return x + y;

	if (stringcmp(op, "-") == 0)
		return x - y;

	if (stringcmp(op, "*") == 0)
		return x * y;

	if (stringcmp(op, "/") == 0)
		return x / y;

	if (stringcmp(op, "%") == 0)
		return x % y;

	return 0;
}
