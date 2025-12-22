/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:23 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/22 20:39:02 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "token_fixtures.h"
#include "shell.h"
#include "shell_node.h"
#include "shell_utils.h"
#include "printers.h"
#include "constants.h"
#include <errno.h>

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_status;
	int		is_print;
	t_token	*tokens;

	// 1 - Print tokens
	// 2 - Print shell_tree
	// 3 - Print both
	// 4 - Print both without execution shell
	is_print = 0;
	if (argc > 1 && argv[1][0] == '1')
		is_print = 1;
	else if (argc > 1 && argv[1][0] == '2')
		is_print = 2;
	else if (argc > 1 && argv[1][0] == '3')
		is_print = 3;
	else if (argc > 1 && argv[1][0] == '4')
		is_print = 4;
	// ( echo foo ; echo bar ) > out | ls -la
	shell = create_shell(envp);
	exit_status = 0;
	while (1)
	{
		tokens = read_and_lexicalize();

		if (!tokens)
			break ;

		if (is_print == 1 || is_print == 3 || is_print == 4)
			print_tokens_brief_once(tokens, shell->ctx->env);

		shell->build(shell, &tokens);
		free_tokens(tokens);
		tokens = NULL;
		if (is_print == 2 || is_print == 3 || is_print == 4)
			print_shell_tree(shell);
		if (is_print == 4)
		{
			shell->clear(shell);
			continue ;
		}

		int heredoc_status = shell->collect_heredoc(shell);
		if (heredoc_status == EXIT_TERMINATED_BY_SIGINT)
		{
    		// Heredoc was interrupted, don't execute
    		shell->ctx->last_exit_status = EXIT_TERMINATED_BY_SIGINT;
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
	return (exit_status);
}
