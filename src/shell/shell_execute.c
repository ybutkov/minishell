/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:51:39 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/22 13:01:36 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "shell.h"
#include "shell_internal.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void	close_fds(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
}

static int	execute_left(t_ast_node *node, t_shell *shell, int in_fd,
		int pipe_fds[2])
{
	close(pipe_fds[0]);
	if (node)
		execute_shell_node(node, shell, in_fd, pipe_fds[1]);
	close(pipe_fds[1]);
	shell->free(shell);
	exit(EXIT_SUCCESS);
}

int	execute_pipe(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int		status_code;
	int		pipe_fds[2];
	pid_t	left_pid;
	int		left_status;

	status_code = 0;
	if (pipe(pipe_fds) == -1)
		return (EXIT_FAILURE);
	left_pid = fork();
	if (left_pid == -1)
	{
		perror("fork");
		close_fds(pipe_fds);
		return (EXIT_FAILURE);
	}
	if (left_pid == 0)
		return (execute_left(node->get_left(node), shell, in_fd, pipe_fds));
	close(pipe_fds[1]);
	if (node->get_right(node))
		status_code = execute_shell_node(node->get_right(node), shell,
				pipe_fds[0], out_fd);
	close(pipe_fds[0]);
	waitpid(left_pid, &left_status, 0);
	return (status_code);
}

int	execute_and(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int		status_code;

	status_code = 0;
	(void)node;
	(void)shell;
	(void)in_fd;
	(void)out_fd;
	return (status_code);
}

int	execute_or(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int		status_code;

	status_code = 0;
	(void)node;
	(void)shell;
	(void)in_fd;
	(void)out_fd;
	return (status_code);
}

int	execute_simicolon(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int		status_code;

	status_code = 0;
	(void)node;
	(void)shell;
	(void)in_fd;
	(void)out_fd;
	return (status_code);
}

int	execute_shell_node(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	t_shell_node	*shell_node;


	shell_node = (t_shell_node *)node->get_content(node);
	if (shell_node->type == NODE_PIPE)
		return (execute_pipe(node, shell, in_fd, out_fd));
	if (shell_node->type == NODE_SEMICOLON)
		return (execute_simicolon(node, shell, in_fd, out_fd));
	if (shell_node->type == NODE_AND)
		return (execute_and(node, shell, in_fd, out_fd));
	else if (shell_node->type == NODE_OR)
		return (execute_or(node, shell, in_fd, out_fd));
	else if (shell_node->type == NODE_CMD)
		return (execute_cmd(shell_node->data.cmd, shell, in_fd, out_fd));
	else if (shell_node->type == NODE_REDIR_HEREDOC)
		return (execute_redir_heredoc(node, shell, in_fd, out_fd));
	return (1);
}
