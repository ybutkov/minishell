/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:23 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 04:20:24 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "shell_node.h"
#include "shell_utils.h"
#include "printers.h"
#include "constants.h"
#include "error.h"
#include <errno.h>

static int	minishell(char **envp)
{
	t_shell	*shell;
	int		exit_status;
	t_token	*tokens;

	shell = create_shell(envp);
	exit_status = 0;
	while (shell->ctx->should_exit == NO)
	{
		tokens = read_and_lexicalize();
		if (!tokens)
			break ;
		shell->build(shell, &tokens);
		free_tokens(tokens);
		tokens = NULL;
		if (shell->collect_heredoc(shell) == EXIT_TERMINATED_BY_SIGINT)
			shell->ctx->last_exit_status = EXIT_TERMINATED_BY_SIGINT;
		else
			shell->execute(shell);
		clear_tmp_folder(get_file_n(0));
		exit_status = shell->ctx->last_exit_status;
		shell->clear(shell);
	}
	shell->free(shell);
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc > 1)
		output_error_and_exit(MSG_INVALID_PARAMS, MSG_REQUIRED_PARAMS, NULL,
			EXIT_FAILURE);
	return (minishell(envp));
}
