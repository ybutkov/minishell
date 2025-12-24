/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_wrapper_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:48:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 02:49:53 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include "error.h"
#include "shell_utils.h"
#include <fcntl.h>

static int	wrapper_exit(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)in_fd;
	(void)out_fd;
	return (bi_exit(shell, cmd->argv));
}

static int	wrapper_export(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)in_fd;
	(void)out_fd;
	return (bi_export(shell->ctx->env, cmd->argv));
}

static int	wrapper_unset(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)in_fd;
	(void)out_fd;
	return (bi_unset(shell->ctx->env, cmd->argv));
}

t_builtin_wrapper	bi_function(int bi_func)
{
	static t_builtin_wrapper	wrappers[7];
	static int					initialized;

	if (!initialized)
	{
		wrappers[0] = wrapper_echo;
		wrappers[1] = wrapper_cd;
		wrappers[2] = wrapper_pwd;
		wrappers[3] = wrapper_export;
		wrappers[4] = wrapper_unset;
		wrappers[5] = wrapper_env;
		wrappers[6] = wrapper_exit;
		initialized = 1;
	}
	if (bi_func >= 0 && bi_func < 7)
		return (wrappers[bi_func]);
	return (NULL);
}

int	builtin(int bi_func, t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	t_builtin_wrapper	wrapper;
	int					saved_stdin;
	int					saved_stdout;
	int					result_status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (EXIT_FAILURE);
	dup2_and_close_both(in_fd, out_fd);
	apply_cmd_redirects(cmd, shell);
	wrapper = bi_function(bi_func);
	if (wrapper)
		result_status = wrapper(cmd, shell, STDIN_FILENO, STDOUT_FILENO);
	else
		result_status = 1;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (result_status);
}
