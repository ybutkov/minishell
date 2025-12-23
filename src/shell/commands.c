/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:26:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 03:00:31 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "shell_internal.h"
#include "parsing.h"
#include "shell_utils.h"

static void	free_cmd(t_cmd *cmd);

static void	free_redir_bridge(void *content)
{
	t_redir	*redir;

	if (!content)
		return ;
	redir = (t_redir *)content;
	redir->free_redir(redir);
}

t_cmd	*create_cmd(char **argv, char *path)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = argv;
	cmd->path = path;
	cmd->redirs = NULL;
	cmd->tokens = NULL;
	cmd->free_cmd = free_cmd;
	return (cmd);
}

static void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->path)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	free_str_array(cmd->argv);
	if (cmd->redirs)
		ft_lstclear(&cmd->redirs, free_redir_bridge);
	free_tokens(cmd->tokens);
	free(cmd);
}
