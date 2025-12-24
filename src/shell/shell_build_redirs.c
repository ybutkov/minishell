/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build_redirs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:53:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 20:42:29 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "libft.h"
#include "parsing.h"
#include "shell_internal.h"
#include "utils.h"

static int	is_redirect_type(t_token *curr, t_token_type type)
{
	if (curr->type == type && curr->next && curr->next->type == TOKEN_WORD)
		return (OK);
	else
		return (NO);
}

t_redir_type	get_only_redir_types(t_token *token)
{
	if (is_redirect_type(token, TOKEN_REDIR_OUT))
		return (REDIR_OUT);
	else if (is_redirect_type(token, TOKEN_REDIR_APPEND))
		return (REDIR_APPEND);
	else if (is_redirect_type(token, TOKEN_REDIR_IN))
		return (REDIR_IN);
	else if (is_redirect_type(token, TOKEN_HEREDOC))
		return (REDIR_HEREDOC);
	else
		return ((t_redir_type)(-1));
}

static void	cut_redir_tokens(t_token **curr_tkn, t_token **start_tkn,
		t_token **end_tkn, int *running)
{
	t_token	*tmp_tkn;

	tmp_tkn = (*curr_tkn)->next->next;
	if ((*curr_tkn)->prev)
		(*curr_tkn)->prev->next = tmp_tkn;
	if (tmp_tkn)
		tmp_tkn->prev = (*curr_tkn)->prev;
	if ((*curr_tkn)->next == (*end_tkn))
	{
		*running = 0;
		(*end_tkn) = (*curr_tkn)->prev;
	}
	if (*curr_tkn == *start_tkn)
		*start_tkn = tmp_tkn;
	(*curr_tkn)->next->free((*curr_tkn)->next);
	(*curr_tkn)->free(*curr_tkn);
	(*curr_tkn) = tmp_tkn;
}

t_redir	*create_redirect(t_shell *shell, t_redir_type type, t_token *curr_tkn)
{
	char	*target;
	t_redir	*redirect;

	target = collect_pieces_to_string(shell, curr_tkn);
	if (target == NULL)
		return (NULL);
	redirect = create_redir(type, target, curr_tkn->stat);
	free(target);
	return (redirect);
}

int	collect_redirs(t_shell *shell, t_cmd *cmd, t_token **start_tkn,
		t_token **end_tkn)
{
	t_redir			*redirect;
	t_redir_type	type;
	int				running;
	t_token			*curr_tkn;

	running = 1;
	curr_tkn = *start_tkn;
	while (running && curr_tkn && curr_tkn != *end_tkn
		&& curr_tkn->type != TOKEN_END)
	{
		type = get_only_redir_types(curr_tkn);
		if (type == (t_redir_type)(-1))
		{
			curr_tkn = curr_tkn->next;
			continue ;
		}
		if (curr_tkn->next == NULL || curr_tkn->next->type != TOKEN_WORD)
			return (ERROR);
		redirect = create_redirect(shell, type, curr_tkn->next);
		add_redirect(cmd, redirect);
		cut_redir_tokens(&curr_tkn, start_tkn, end_tkn, &running);
	}
	return (OK);
}
