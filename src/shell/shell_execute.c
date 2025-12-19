/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:51:39 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/19 13:33:58 by ybutkov          ###   ########.fr       */
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
#include "signals.h"

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

static int	execute_right(t_ast_node *node, t_shell *shell, int pipe_fds[2], int out_fd)
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

int	execute_pipe(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int		pipe_fds[2];
	pid_t	pids[2];
	int		status;

	if (pipe(pipe_fds) == -1)
		return (EXIT_FAILURE);
	set_signals_waiting_parent();
	pids[0] = fork();
	if (pids[0] == -1)
	{
		perror("fork");
		close_fds(&pipe_fds[0], &pipe_fds[1]);
		return (EXIT_FAILURE);
	}
	if (pids[0] == 0)
	{
		set_signals_child();
		return (execute_left(node->get_left(node), shell, in_fd, pipe_fds));
	}
	close_fds(NULL, &pipe_fds[1]);
	pids[1] = fork();
	if (pids[1] == -1)
	{
		perror("fork");
		close_fds(&pipe_fds[0], &pipe_fds[1]);
		return (EXIT_FAILURE);
	}
	if (pids[1] == 0)
	{
		set_signals_child();
		return (execute_right(node->get_right(node), shell, pipe_fds, out_fd));
	}
	close_fds(&pipe_fds[0], NULL);
	waitpid(pids[0], &status, 0);
	waitpid(pids[1], &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}

int	execute_and(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int	code;

	code = 0;
	if (node->get_left(node))
		code = execute_shell_node(node->get_left(node), shell, in_fd, out_fd);
	if (code == 0 && node->get_right(node))
		code = execute_shell_node(node->get_right(node), shell, in_fd, out_fd);
	return (code);
}

int	execute_or(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int		code;

	code = 0;
	if (node->get_left(node))
		code = execute_shell_node(node->get_left(node), shell, in_fd, out_fd);
	if (code != 0 && node->get_right(node))
		code = execute_shell_node(node->get_right(node), shell, in_fd, out_fd);
	return (code);
}

int	execute_simicolon(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int	code;

	if (node->get_left(node))
		code = execute_shell_node(node->get_left(node), shell, in_fd, out_fd);
	if (node->get_right(node))
		code = execute_shell_node(node->get_right(node), shell, in_fd, out_fd);
	return (code);
}

static void	apply_subshell_redirs(t_shell_node *shell_node)
{
	t_list	*redir_list;
	t_redir	*redirect;
	int		fd;

	redir_list = shell_node->redirs;
	while (redir_list)
	{
		redirect = (t_redir *)redir_list->content;
		if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
		{
			fd = open(redirect->target, O_RDONLY);
			if (fd != -1)
			{
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		else if (redirect->type == REDIR_OUT)
		{
			fd = open(redirect->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd != -1)
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
		else if (redirect->type == REDIR_APPEND)
		{
			fd = open(redirect->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd != -1)
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
		redir_list = redir_list->next;
	}
}

int	execute_subshell(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	pid_t			pid;
	int				status;
	t_shell_node	*shell_node;

	set_signals_waiting_parent();
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		set_signals_child();
		if (in_fd != STDIN_FILENO)
			dup2_and_close(in_fd, STDIN_FILENO);
		if (out_fd != STDOUT_FILENO)
			dup2_and_close(out_fd, STDOUT_FILENO);
		else
			dup2(STDOUT_FILENO, STDOUT_FILENO);
		shell_node = (t_shell_node *)node->get_content(node);
		if (shell_node && shell_node->redirs)
			apply_subshell_redirs(shell_node);
		status = execute_shell_node(node->get_left(node), shell,
				STDIN_FILENO, STDOUT_FILENO);
		shell->free(shell);
		exit(status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}

int	execute_shell_node(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	t_shell_node	*shell_node;

	shell_node = (t_shell_node *)node->get_content(node);
	// create array of wrapper funcs
	if (shell_node->type == NODE_SUBSHELL)
		return (execute_subshell(node, shell, in_fd, out_fd));
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
	return (1);
}
