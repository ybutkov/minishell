/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_tree.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:52:06 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 04:35:34 by ybutkov          ###   ########.fr       */
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

static int	execute_and(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int	code;

	code = 0;
	if (node->get_left(node))
		code = execute_shell_node(node->get_left(node), shell, in_fd, out_fd);
	if (code == 0 && node->get_right(node))
		code = execute_shell_node(node->get_right(node), shell, in_fd, out_fd);
	return (code);
}

static int	execute_or(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	int	code;

	code = 0;
	if (node->get_left(node))
		code = execute_shell_node(node->get_left(node), shell, in_fd, out_fd);
	if (code != 0 && node->get_right(node))
		code = execute_shell_node(node->get_right(node), shell, in_fd, out_fd);
	return (code);
}

static int	execute_simicolon(t_ast_node *node, t_shell *shell, int in_fd,
		int out_fd)
{
	int	code;

	if (node->get_left(node))
		code = execute_shell_node(node->get_left(node), shell, in_fd, out_fd);
	if (node->get_right(node))
		code = execute_shell_node(node->get_right(node), shell, in_fd, out_fd);
	return (code);
}

static int	execute_subshell(t_ast_node *node, t_shell *shell, int in_fd,
		int out_fd)
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
		dup2_and_close_both(in_fd, out_fd);
		shell_node = (t_shell_node *)node->get_content(node);
		if (shell_node && shell_node->redirs)
			apply_subshell_redirs(shell, shell_node);
		status = execute_shell_node(node->get_left(node), shell, STDIN_FILENO,
				STDOUT_FILENO);
		shell->free(shell);
		exit(status);
	}
	waitpid(pid, &status, 0);
	return (return_status(status));
}

int	execute_shell_node(t_ast_node *node, t_shell *shell, int in_fd, int out_fd)
{
	t_shell_node	*shell_node;

	shell_node = (t_shell_node *)node->get_content(node);
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
