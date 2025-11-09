/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:26:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/09 19:20:07 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "shell_internal.h"
#include "parcer.h"
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
	cmd->free_cmd = free_cmd;
	return (cmd);
}

t_cmd	*create_cmd_from_raw_str(char *raw_command, t_shell *shell)
{
	char	**cmd_argv;
	char	*full_path;

	cmd_argv = parse_command(raw_command);
	if (!cmd_argv)
		return (NULL);
	full_path = get_cmd_path(cmd_argv[0], shell->ctx->envp);
	return (create_cmd(cmd_argv, full_path));
}

static void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_str_array(cmd->argv);
	if (cmd->path)
		free(cmd->path);
	if (cmd->redirs)
		ft_lstclear(&cmd->redirs, free_redir_bridge);
	free(cmd);
}
