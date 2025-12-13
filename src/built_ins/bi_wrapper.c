/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_wrapper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:48:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/08 12:41:04 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include "error.h"
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

// TEMP should create one file for redirect funcs
static void	open_file_and_dup2(char *filename, int flags, int dup_fd,
		t_shell *shell)
{
	int	fd;
	int	mode;

	if (flags & O_CREAT)
		mode = 0644;
	else
		mode = 0;
	fd = open(filename, flags, mode);
	if (fd == -1)
		output_error_and_exit(filename, NULL, shell, EXIT_FAILURE);
	dup2(fd, dup_fd);
	close(fd);
}

// TEMP should create one file for redirect funcs
static void	apply_redirect(t_cmd *cmd, t_shell *shell)
{
	t_list	*redir;
	t_redir	*redirect;

	if (!cmd)
		return ;
	redir = cmd->redirs;
	while (redir)
	{
		redirect = (t_redir *)redir->content;
		if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
			open_file_and_dup2(redirect->target, O_RDONLY, STDIN_FILENO, shell);
		else if (redirect->type == REDIR_OUT)
			open_file_and_dup2(redirect->target, O_WRONLY | O_CREAT | O_TRUNC,
				STDOUT_FILENO, shell);
		else if (redirect->type == REDIR_APPEND)
			open_file_and_dup2(redirect->target, O_WRONLY | O_CREAT | O_APPEND,
				STDOUT_FILENO, shell);
		redir = redir->next;
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
