#include "utils.h"
#include "mpc.h"

#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>

static struct lval *lval_read(mpc_ast_t *ast);
static struct lval *lval_read_num(mpc_ast_t *ast);

int main(void)
{
	mpc_parser_t *Number = mpc_new("number");
	mpc_parser_t *Symbol = mpc_new("symbol");
	mpc_parser_t *Sexpr = mpc_new("sexpr");
	mpc_parser_t *Qexpr = mpc_new("qexpr");
	mpc_parser_t *Expr = mpc_new("expr");
	mpc_parser_t *Sclisp = mpc_new("sclisp");

	mpc_parser_t *init_parsers[] = {
		Number,
		Symbol,
		Sexpr,
		Qexpr,
		Expr,
		Sclisp
	};

	size_t parsers_len = sizeof(init_parsers) / sizeof(init_parsers[0]);

	mpca_lang(
		MPCA_LANG_DEFAULT,
		"number: /-?\\d+(\\.\\d+)?/;"
		"symbol: \"head\" | \"tail\" | \"list\" | \"eval\" | \"join\" | \"len\" | '+' | '-' | '*' | '/' | '%' | '^';"
		"sexpr: '(' <expr>* ')';"
		"qexpr: '{' <expr>* '}';"
		"expr: <number> | <symbol> | <sexpr> | <qexpr>;"
		"sclisp: /^/ <expr>* /$/;",
		parsers_len, init_parsers
	);

	puts("Sclisp v0.0.1-rc2");
	puts("Press ctrl-c, ctrl-d, exit command to exit\n");

	const char *prompt;
	char *input;

	while (1) {
		prompt = "sclisp> ";
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

			mpc_ast_delete(ast);
			ast = NULL;
		} else {
			mpc_err_print(result.error);
			mpc_err_delete(result.error);
		}

		free(input);
	}

	mpc_cleanup(parsers_len, init_parsers);
	return 0;
}

static struct lval *lval_read(mpc_ast_t *ast)
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
	if (*ast->tag == '>' || strstr(ast->tag, "sexpr"))
		value = lval_sexpr();

	if (strstr(ast->tag, "qexpr"))
		value = lval_qexpr();

	for (i = 0; i < ast->children_num; i++) {
		if (*ast->children[i]->contents == '('
			|| *ast->children[i]->contents == ')'
			|| *ast->children[i]->contents == '{'
			|| *ast->children[i]->contents == '}'
			|| stringcmp(ast->children[i]->tag, "regex") == 0
		)
			continue;

		value = lval_add(
			value,
			lval_read(ast->children[i])
		);
	}

	return value;
}

static struct lval *lval_read_num(mpc_ast_t *ast)
{
	double num = strtod(ast->contents, NULL);

	return num == 0 && errno == ERANGE
		? lval_err("invalid number")
		: lval_num(num);
}
