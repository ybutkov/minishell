/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_wrapper_further.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 23:42:38 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/23 23:55:45 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include "error.h"
#include "shell_utils.h"
#include <fcntl.h>

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
