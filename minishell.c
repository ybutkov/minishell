/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:23 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/23 19:10:49 by ybutkov          ###   ########.fr       */
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
	// test_shell(tokens_simple_pipe(), envp,
	// 	"ls | wc -l", isprint);
	// test_shell(tokens_echo_grep_redirect(), envp,
	// 	"echo \"Hello World\" | grep Hello > out.txt", isprint);
	// test_shell(tokens_pipe_with_redirects(), envp,
	// 	"cat NEW_TOKEN_FIXTURES.md | grep WORD > output.txt", isprint);
	// test_shell(tokens_five_pipe_chain(), envp,
	// 	"cat minishell.c | grep int | sort | uniq | wc -l", isprint);
	// test_shell(tokens_triple_pipe(), envp,
	// 	"ls | grep i | wc -l", isprint);
	// test_shell(tokens_triple_pipe_error(), envp,
	// 	"ls | grep i | wc -tt", isprint);
	// test_shell(tokens_input_pipe_chain(), envp,
	// 	"cat < minishell_test.log | grep home | wc -l", isprint);
	// test_shell(tokens_input_pipe_chain_1_redir(), envp,
	// 	"< minishell_test.log cat  | grep home | wc -l", isprint);
	// test_shell(tokens_dual_redirect_pipe(), envp,
	// 	"cat < NEW_TOKEN_FIXTURES.md > output_21.txt | wc -l", isprint);
	// test_shell(tokens_complex_multi_redirect(), envp,
	// 	"ls -la > out_22.txt < /dev/null | grep ell >> out_23.txt", isprint);
	// test_shell(tokens_complex_multi_redirect_2(), envp,
	// 	"ls -la > out_21.txt > out_22.txt | <out_22.txt grep ell >> out_23.txt",
	// 	isprint);
	// test_shell(tokens_single_command(), envp, "ls -la > out_.txt", isprint);
	// echo 'single quotes'"and double" ; printf "%s\n" "done" >> log.txt
	// test_shell(tokens_quotes_semi_append(), envp,
	// 	"echo 'single quotes'\"and double\" ; printf \"%s\n\" \"done\" >> log.txt",
	// 	isprint);
	// ls ; pwd ; echo done
	// test_shell(tokens_multiple_semicolon(), envp, "ls ; pwd ; echo done", isprint);
	// echo test > out.txt && cat out.txt
	test_shell(tokens_redirect_and_command(), envp, "echo test > out.txt && cat out.txt",
		isprint);
	// echo hello && echo world
	test_shell(tokens_and_commands(), envp, "echo hello && echo world", isprint);


}

int	main(int argc, char **argv, char **envp)
{
	// t_shell	*shell;
	// int		exit_status;

	(void)argc;
	(void)argv;

	test_shells(envp, 1);
	return (0);
}
