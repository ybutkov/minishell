/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:51:50 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 02:51:54 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"
#include "shell_internal.h"
#include "shell_utils.h"
#include "signals.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static int	execute_left(t_ast_node *node, t_shell *shell, int in_fd,
		int pipe_fds[2])
{
	int	exit_status;

	exit_status = 0;
	if (pipe_fds[1] != STDOUT_FILENO)
		dup2_and_close(pipe_fds[1], STDOUT_FILENO);
	close_fds(&pipe_fds[0], &pipe_fds[1]);
	if (node)
		exit_status = execute_shell_node(node, shell, in_fd, STDOUT_FILENO);
	shell->free(shell);
	exit(exit_status);
}

static int	execute_right(t_ast_node *node, t_shell *shell, int pipe_fds[2],
		int out_fd)
{
	int	exit_status;

	exit_status = 0;
	if (pipe_fds[0] != STDIN_FILENO)
		dup2_and_close(pipe_fds[0], STDIN_FILENO);
	close_fds(&pipe_fds[0], &pipe_fds[1]);
	if (node)
		exit_status = execute_shell_node(node, shell, STDIN_FILENO, out_fd);
	shell->free(shell);
	exit(exit_status);
}

static int	error_close_return(int *fd_0, int *fd_1)
{
	output_error(FORK_ERROR, NULL);
	close_fds(fd_0, fd_1);
	return (EXIT_FAILURE);
}

int	execute_pipe(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int		pipe_fds[2];
	pid_t	pids[2];
	int		status;

	if (pipe(pipe_fds) == -1)
		return (EXIT_FAILURE);
	set_signals_waiting_parent();
	pids[0] = fork();
	if (pids[0] < 0)
		return (error_close_return(&pipe_fds[0], &pipe_fds[1]));
	if (pids[0] == 0)
		return (set_signals_child(), execute_left(node->get_left(node), shell,
				in_fd, pipe_fds));
	close_fds(NULL, &pipe_fds[1]);
	pids[1] = fork();
	if (pids[1] < 0)
		return (error_close_return(&pipe_fds[0], &pipe_fds[1]));
	if (pids[1] == 0)
		return (set_signals_child(), execute_right(node->get_right(node), shell,
				pipe_fds, out_fd));
	close_fds(&pipe_fds[0], NULL);
	waitpid(pids[0], &status, 0);
	waitpid(pids[1], &status, 0);
	return (return_status(status));
}
