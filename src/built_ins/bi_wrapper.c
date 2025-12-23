/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_wrapper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:48:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 23:42:11 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include "error.h"
#include "shell_utils.h"
#include <fcntl.h>

int	wrapper_export(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)in_fd;
	(void)out_fd;
	return (bi_export(shell->ctx->env, cmd->argv));
}

int	wrapper_unset(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
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

void	checking_input_output(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
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
	checking_input_output(in_fd, out_fd);
	apply_redirect(cmd, shell);
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
