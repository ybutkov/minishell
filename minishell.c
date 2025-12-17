/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:23 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/17 19:15:13 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_fixtures.h"
#include "shell.h"
#include "shell_node.h"
#include "shell_utils.h"
#include "printers.h"
#include <errno.h>

int	test_shell(t_token *tokens, char **envp, char *test_comm, int isprint)
{
	t_shell	*shell;
	int		exit_status;

	if (isprint)
		printf("\n--- Test: %s ---\n", test_comm);
	shell = create_shell(envp);
	shell->build(shell, tokens);

	print_shell_tree(shell);
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
	// 	"ls -la > out_21.txt > out_22.txt | <out_22.txt grep ell >> out_23.txt",
	// 	isprint);
	// test_shell(tokens_single_command(), envp, "ls -la > out_.txt", isprint);
	// // echo 'single quotes'"and double" ; printf "%s\n" "done" >> log.txt
	// test_shell(tokens_quotes_semi_append(), envp,
	// 	"echo 'single quotes'\"and double\" ; printf \"%s\n\" \"done\" >> log.txt",
	// 	isprint);
	// test_shell(tokens_multiple_semicolon(), envp, "ls ; pwd ; echo done", isprint);
	// test_shell(tokens_redirect_and_command(), envp, "echo test > out.txt && cat out.txt",
	// 	isprint);
	// test_shell(tokens_and_commands(), envp, "echo hello && echo world", isprint);
	// test_shell(tokens_mixed_pipe_redir(), envp,
	// 	"cat tests/in/lines.txt | sort > tests/out/sorted_lines.txt", isprint);

	// test_shell(tokens_group_and_pipe(), envp, "(cd /tmp && ls -la) | wc -l", isprint);

	// test_shell(tokens_paren_echo_redirect(), envp, "(echo hello) > out_paren.txt", isprint);
	// test_shell(tokens_paren_ls(), envp, "(ls -la)", isprint);
	test_shell(tokens_paren_group_semicolon(), envp, "(echo foo ; echo bar)", isprint);
	// test_shell(tokens_paren_group_semicolon_pipe(), envp, "(echo foo ; echo bar) | wc -l", isprint);
	// test_shell(tokens_paren_pwd_cd_chain_pipe(), envp, "(pwd; cd ..; pwd) | (cd .. && pwd && cd .. && pwd)", isprint);

}


int	check_for_echo_$(t_token *tokens, int exit_status)
{
	if (tokens->type == TOKEN_WORD
		&& ft_strcmp(tokens->value, "echo") == 0)
	{
		if (tokens->next->type == TOKEN_WORD
		&& ft_strcmp(tokens->next->value, "$?") == 0)
		{
			printf("%d\n", exit_status);
			return (1);
		}
	}
	return (0);
}

void test_env(char **envp)
{
	t_ctx *ctx = create_ctx(envp);
	t_env *env = ctx->env;
	char *str = env->get_value(env, "LANG");
	printf("LANG=%s\n", str);
	env->set_pair(env, "HHH", "value");
	str = env->get_value(env, "HHH");
	printf("HHH=%s\n", str);
	env->remove_pair(env, "HHH");
	str = env->get_value(env, "HHH");
	printf("HHH=%s\n", str);

	exit (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_status;
	int		is_print;

	// 1 - Print tokens
	// 2 - Print shell_tree
	// 3 - Print both
	// 4 - Print both without execution shell
	// 5 - run test_shell
	// 6 - test env
	is_print = 0;
	if (argc > 1 && argv[1][0] == '1')
		is_print = 1;
	else if (argc > 1 && argv[1][0] == '2')
		is_print = 2;
	else if (argc > 1 && argv[1][0] == '3')
		is_print = 3;
	else if (argc > 1 && argv[1][0] == '4')
		is_print = 4;
	else if (argc > 1 && argv[1][0] == '5')
		is_print = 5;
	else if (argc > 1 && argv[1][0] == '6')
		is_print = 6;

	if (is_print == 6)
		test_env(envp);
	// ( echo foo ; echo bar ) > out | ls -la
	if (is_print == 5)
		return (test_shells(envp, 1), 0);
	shell = create_shell(envp);
	exit_status = 0;
	while (1)
	{
		t_token *tokens = read_and_lexicalize();

		if (!tokens)
			break ;

		if (is_print == 1 || is_print == 3 || is_print == 4)
			print_tokens_brief_once(tokens, shell->ctx->env);

		// print_tokens_brief_once(tokens);
		//*************************************** */
		// if (check_for_echo_$(tokens, exit_status) == 1)
		// 	continue ;
		shell->build(shell, tokens);

		if (is_print == 2 || is_print == 3 || is_print == 4)
			print_shell_tree(shell);
		if (is_print == 4)
		{
			shell->clear(shell);
			continue ;
		}

		int heredoc_status = shell->collect_heredoc(shell);
		if (heredoc_status == 130)
		{
    		// Heredoc was interrupted, don't execute
    		shell->ctx->last_exit_status = 130;
		}
		else
		{
    		shell->execute(shell);
		}
		clear_tmp_folder(get_file_n(0));
		exit_status = shell->ctx->last_exit_status;
		shell->clear(shell);
		if (shell->ctx->should_exit)
			break ;
	}
	shell->free(shell);
	char *f = malloc(100);
	(void)f;
	return (exit_status);
}
