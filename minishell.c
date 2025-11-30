/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:23 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/30 15:13:23 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_fixtures.h"
#include "shell.h"
#include "print_shell.h"

int	test_shell(t_token *tokens, char **envp, char *test_comm, int isprint)
{
	t_shell	*shell;
	int		exit_status;

	if (isprint)
		printf("\n--- Test: %s ---\n", test_comm);
	shell = create_shell(envp);
	shell->build(shell, tokens);

	// print_shell_tree(shell);
	printf("---------------------------------------------------------------\n");
	shell->execute(shell);
	exit_status = shell->ctx->last_exit_status;
	shell->free(shell);
	printf("---------------------------------------------------------------\n");
	if (isprint)
		printf("Exit status: %d\n", exit_status);
	printf("==================== End test =================================\n\n");
	return (exit_status);
}

void test_shells(char **envp, int isprint)
{
	test_shell(tokens_simple_pipe(), envp,
		"ls | wc -l", isprint);
	test_shell(tokens_echo_grep_redirect(), envp,
		"echo \"Hello World\" | grep Hello > out.txt", isprint);
	test_shell(tokens_pipe_with_redirects(), envp,
		"cat NEW_TOKEN_FIXTURES.md | grep WORD > output.txt", isprint);
	test_shell(tokens_five_pipe_chain(), envp,
		"cat minishell.c | grep int | sort | uniq | wc -l", isprint);
	test_shell(tokens_triple_pipe(), envp,
		"ls | grep i | wc -l", isprint);
	test_shell(tokens_triple_pipe_error(), envp,
		"ls | grep i | wc -tt", isprint);
	test_shell(tokens_input_pipe_chain(), envp,
		"cat < minishell_test.log | grep home | wc -l", isprint);
	test_shell(tokens_input_pipe_chain_1_redir(), envp,
		"< minishell_test.log cat  | grep home | wc -l", isprint);
	test_shell(tokens_dual_redirect_pipe(), envp,
		"cat < NEW_TOKEN_FIXTURES.md > output_21.txt | wc -l", isprint);
	test_shell(tokens_complex_multi_redirect(), envp,
		"ls -la > out_22.txt < /dev/null | grep ell >> out_23.txt", isprint);
	test_shell(tokens_complex_multi_redirect_2(), envp,
		"ls -la > out_21.txt > out_22.txt | <out_22.txt grep ell >> out_23.txt",
		isprint);

}
// static void print_tokens_brief_once(t_token *toks)
// {
//     t_token *t;
//     t_piece *p;
//     int ti;
//     int pi;

//     if (!toks)
//     {
//         printf("no tokens\n");
//         return;
//     }
//     ti = 0;
//     for (t = toks; t; t = t->next, ++ti)
//     {
//         printf("Token[%d]: '%s'\n", ti, t->value ? t->value : "(null)");
//         if (!t->pieces)
//             continue;
//         pi = 0;
//         for (p = t->pieces; p; p = p->next, ++pi)
//             printf("  piece[%d] q=%d text='%s'\n",
//                 pi,
//                 (int)p->quotes,
//                 p->text ? p->text : "(null)");
//     }
// }

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_status;

	(void)argc;
	(void)argv;
	(void)envp;

	shell = create_shell(envp);
	exit_status = 0;
	while (1)
	{
		t_token *tokens = read_and_lexicalize();
		if (!tokens)
			break ;
		shell->build(shell, tokens);
		// print_shell_tree(shell);
		// printf("---------------------------------------------------------------\n");
		shell->execute(shell);
		exit_status = shell->ctx->last_exit_status;
		
	}
	shell->free(shell);
	// test_shells(envp, 1);
	// print_tokens_brief_once(toks);
	return (0);
}
