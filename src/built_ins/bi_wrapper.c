/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_wrapper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:48:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/07 03:56:26 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

int	wrapper_pwd(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)cmd;
	(void)shell;
	(void)in_fd;
	(void)out_fd;
	return (bi_pwd());
}

int	wrapper_cd(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)in_fd;
	(void)out_fd;
	return (bi_cd(shell->ctx->env, cmd->argv));
}

int	wrapper_echo(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)shell;
	(void)in_fd;
	(void)out_fd;
	return (bi_echo(cmd->argv));
}

int	wrapper_env(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)cmd;
	(void)in_fd;
	(void)out_fd;
	return (bi_env(shell->ctx->env));
}

int	wrapper_exit(t_cmd *cmd, t_shell *shell, int in_fd, int out_fd)
{
	(void)in_fd;
	(void)out_fd;
	return (bi_exit(shell, cmd->argv));
}

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

t_builtin_wrapper bi_function(int bi_func)
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

	// add collect_redir

	wrapper = bi_function(bi_func);
	if (wrapper)
		return (wrapper(cmd, shell, in_fd, out_fd)); // STDIN_FILENO, STDOUT_FILENO ??
	return (1);
}

