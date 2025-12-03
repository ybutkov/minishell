/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:23 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/03 20:38:29 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_fixtures.h"
#include "shell.h"
#include "print_shell.h"
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
static void print_tokens_brief_once(t_token *toks)
{
    t_token *t;
    t_piece *p;
    int ti;
    int pi;

    if (!toks)
    {
        // printf("no tokens\n");
        return;
    }
    ti = 0;
    for (t = toks; t; t = t->next, ++ti)
    {
        printf("Token[%d]: type=%u '%s'\n", ti, t->type, t->value ? t->value : "(null)");
        if (!t->pieces)
            continue;
		write(1, "GGG\n", 4);
        pi = 0;
        for (p = t->pieces; p; p = p->next, ++pi)
            printf("  piece[%d] q=%d text='%s'\n",
                pi,
                (int)p->quotes,
                p->text ? p->text : "(null)");
    }
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


int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_status;

	(void)argc;
	(void)argv;
	(void)envp;

	print_tokens_brief_once(NULL); // tmp
	// test_shells(envp, 1); // tmp
	// return (0);
	shell = create_shell(envp);
	exit_status = 0;
	while (1)
	{
		t_token *tokens = read_and_lexicalize();
		if (!tokens)
			break ;
		// print_tokens_brief_once(tokens); getchar();
		if (check_for_echo_$(tokens, exit_status) == 1)
			continue ;
		shell = create_shell(envp);
		// write(1, "111\n", 4);
		shell->build(shell, tokens);
		// write(1, "222\n", 4);
		// print_shell_tree(shell); getchar();

		shell->execute(shell);
		exit_status = shell->ctx->last_exit_status;
		shell->clear(shell);
	}
	shell->free(shell);
	// test_shells(envp, 1);
	// print_tokens_brief_once(toks);
	return (0);
}
