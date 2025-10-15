#include "mpc.h"
#include "stringcmp.h"

#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>

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
		"number: /-?[0-9]+/;"
		"operator: '+' | '-' | '*' | '/';"
		"expr: <number> | '(' <operator> <expr>+ ')';"
		"sclisp: /^/ <operator> <expr>+ /$/;",
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
			mpc_ast_print(result.output);
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
