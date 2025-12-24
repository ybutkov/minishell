/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_wrapper_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:48:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 19:05:42 by ybutkov          ###   ########.fr       */
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
