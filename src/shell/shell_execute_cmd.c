/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:57:40 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/09 17:58:35 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "constants.h"
#include "get_next_line.h"
#include "libft.h"
#include "shell.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

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
		if (redirect->type == REDIR_IN)
			open_file_and_dup2(redirect->target, O_RDONLY, STDIN_FILENO, shell);
		else if (redirect->type == REDIR_OUT)
			open_file_and_dup2(redirect->target, O_WRONLY | O_CREAT | O_TRUNC,
				STDOUT_FILENO, shell);
		else if (redirect->type == REDIR_APPEND)
			open_file_and_dup2(redirect->target, O_WRONLY | O_CREAT | O_APPEND,
				STDOUT_FILENO, shell);
		else if (redirect->type == REDIR_HEREDOC)
		{
		}
		redir = redir->next;
	}
}

static int	execute_cmd_child(t_cmd *cmd, t_shell *shell, int input_fd,
		int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	apply_redirect(cmd, shell);
	if (!cmd->path || access(cmd->path, X_OK) != 0)
		output_error_and_exit(cmd->argv[0], CMD_NOT_FOUND_MSG, shell,
			EXIT_CMD_NOT_FOUND);
	execve(cmd->path, cmd->argv, shell->ctx->envp);
	shell->free(shell);
	exit(EXIT_FAILURE);
}

int	execute_cmd(t_cmd *cmd, t_shell *shell, int input_fd, int output_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		output_error_and_exit("fork", NULL, shell, EXIT_FAILURE);
	if (pid == 0)
		return (execute_cmd_child(cmd, shell, input_fd, output_fd));
	else
	{
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		if (output_fd != STDOUT_FILENO)
			close(output_fd);
		waitpid(pid, &status, 0);
		shell->ctx->last_exit_status = WEXITSTATUS(status);
		return (WEXITSTATUS(status));
	}
}
