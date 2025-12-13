/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:23:57 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/09 18:23:59 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	free_redir_content(void *content)
{
	t_redir	*redir;

	redir = (t_redir *)content;
	if (redir)
		redir->free_redir(redir);
}

static void	free_shell_node(t_shell_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_CMD && node->data.cmd)
	{
		node->data.cmd->free_cmd(node->data.cmd);
	}
	else if (node->type == NODE_SUBSHELL)
	{
		if (node->redirs)
			ft_lstclear(&node->redirs, free_redir_content);
	}
	else if ((node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
			|| node->type == NODE_REDIR_APPEND
			|| node->type == NODE_REDIR_HEREDOC)
		&& node->data.redir)
	{
		node->data.redir->free_redir(node->data.redir);
	}
	free(node);
}

t_shell_node	*create_shell_node(t_node_type type, void *data)
{
	t_shell_node	*node;

	node = malloc(sizeof(t_shell_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->redirs = NULL;
	if (type == NODE_CMD)
		node->data.cmd = (t_cmd *)data;
	else if (type == NODE_REDIR_IN || type == NODE_REDIR_OUT
		|| type == NODE_REDIR_APPEND || type == NODE_REDIR_HEREDOC)
		node->data.redir = (t_redir *)data;
	else
	{
		node->data.cmd = NULL;
	}
	node->free = free_shell_node;
	return (node);
}
