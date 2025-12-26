/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_heredoc_tree.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:20:32 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/26 02:59:43 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "error.h"
#include "libft.h"
#include "parsing.h"
#include "shell_internal.h"
#include "shell_utils.h"
#include "signals.h"
#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

static int	execute_redir_heredoc(t_shell *shell, t_redir *redirect)
{
	int		fd;
	int		status;
	char	*file_name;

	file_name = get_tmp_file_name(get_file_n(1));
	if (file_name == NULL)
		return (EXIT_FAILURE);
	fd = open_file(file_name, O_WRONLY | O_CREAT | O_TRUNC, shell);
	status = collect_heredoc_input(redirect, fd, shell);
	close(fd);
	if (status == EXIT_TERMINATED_BY_SIGINT)
	{
		unlink(file_name);
		free(file_name);
		return (EXIT_TERMINATED_BY_SIGINT);
	}
	free(redirect->target);
	redirect->target = file_name;
	return (EXIT_SUCCESS);
}

static int	collect_heredoc_subshell(t_ast_node *node, t_shell *shell)
{
	return (collect_heredoc_node(node->get_left(node), shell));
}

static int	collect_heredoc_double(t_ast_node *node, t_shell *shell)
{
	int	status;

	if (node->get_left(node))
		status = collect_heredoc_node(node->get_left(node), shell);
	if (node->get_left(node))
		status = collect_heredoc_node(node->get_right(node), shell);
	return (status);
}

static int	collect_heredoc_cmd(t_cmd *cmd, t_shell *shell)
{
	t_list	*redir;
	t_redir	*redirect;
	int		status;

	status = 0;
	if (!cmd)
		return (EXIT_FAILURE);
	redir = cmd->redirs;
	while (redir)
	{
		redirect = (t_redir *)redir->content;
		if (redirect->type == REDIR_HEREDOC)
		{
			status = execute_redir_heredoc(shell, redirect);
			if (status == EXIT_TERMINATED_BY_SIGINT)
				return (status);
		}
		redir = redir->next;
	}
	return (status);
}

int	collect_heredoc_node(t_ast_node *node, t_shell *shell)
{
	t_shell_node	*shell_node;

	if (node == NULL)
		return (1);
	shell_node = (t_shell_node *)node->get_content(node);
	if (shell_node->type == NODE_SUBSHELL)
		return (collect_heredoc_subshell(node, shell));
	if (shell_node->type == NODE_PIPE)
		return (collect_heredoc_double(node, shell));
	if (shell_node->type == NODE_SEMICOLON)
		return (collect_heredoc_double(node, shell));
	if (shell_node->type == NODE_AND)
		return (collect_heredoc_double(node, shell));
	else if (shell_node->type == NODE_OR)
		return (collect_heredoc_double(node, shell));
	else if (shell_node->type == NODE_CMD)
		return (collect_heredoc_cmd(shell_node->data.cmd, shell));
	return (1);
}
