/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 19:23:09 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/22 20:15:52 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "shell_utils.h"
#include <fcntl.h>

static void	free_redir_content(t_redir *redir)
{
	if (!redir)
		return ;
	if (redir->target)
		free(redir->target);
	free(redir);
}

t_redir	*create_redir(t_redir_type type, char *target, e_quotes_status quotes)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = ft_strdup(target);
	redir->quotes = quotes;
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	redir->free_redir = free_redir_content;
	return (redir);
}

void	apply_redirects(t_list *redir, t_shell *shell)
{
	t_redir	*redirect;

	if (!redir)
		return ;
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

void	apply_redirect(t_cmd *cmd, t_shell *shell)
{
	t_list	*redir;

	if (!cmd)
		return ;
	redir = cmd->redirs;
	apply_redirects(redir, shell);
}
