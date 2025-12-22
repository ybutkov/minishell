/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 20:29:07 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/21 22:52:14 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "lexer_internal.h"

static t_token	*create_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token_init(token);
	token->has_wild = 0;
	token->has_env_v = 0;
	token->free = free_token;
	return (token);
}

t_piece	*copy_pieces(t_piece *old)
{
	t_piece	*new;
	t_piece	*piece;
	t_piece	*prev;

	new = NULL;
	prev = NULL;
	while (old)
	{
		piece = malloc(sizeof(t_piece));
		if (!piece)
			return (HANDLE_ERROR_NULL);
		piece->text = ft_strdup(old->text);
		piece->next = NULL;
		piece->has_env_v = old->has_env_v;
		piece->has_tilde = old->has_tilde;
		piece->has_wild = old->has_wild;
		piece->quotes = old->quotes;
		if (!new)
			new = piece;
		if (prev)
			prev->next = piece;
		prev = piece;
		old = old->next;
	}
	return (new);
}

t_token	*copy_token(t_token *old)
{
	t_token	*new;

	new = create_token();
	init_token(new, old->type);
	new->stat = old->stat;
	new->value = ft_strdup(old->value);
	new->has_env_v = old->has_env_v;
	new->has_wild = old->has_wild;
	new->pieces = copy_pieces(old->pieces);
	new->pieces_tail = NULL;
	return (new);
}

// Need to check malloc's problems
t_token	*copy_tokens(t_token *start, t_token *end)
{
	t_token	*new;
	t_token	*prev;
	t_token	*tmp;

	new = NULL;
	prev = NULL;
	while (start)
	{
		tmp = copy_token(start);
		if (!new)
			new = tmp;
		if (prev)
			prev->next = tmp;
		prev = tmp;
		if (start == end)
		{
			prev->next = NULL;
			break ;
		}
		start = start->next;
	}
	return (new);
}
