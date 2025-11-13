#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <editline/readline.h>

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
