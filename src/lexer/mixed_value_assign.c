/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixed_value_assign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:45:44 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 03:53:26 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	handle_piece(t_token *t, t_pieces_internal *pi)
{
	if (t->value[pi->i] == '"')
		assign_d_quo_pieces(t, pi);
	else if (t->value[pi->i] == '\'')
		assign_s_quo_pieces(t, pi);
	else if (t->value[pi->i] == '$' || t->value[pi->i] == '*'
		|| t->value[pi->i] == '~')
		assign_env_wild_pieces(t, pi);
	else
	{
		pi->cur_start = pi->i;
		while (is_whitespace(t->value[pi->i]))
			pi->i++;
		while (!is_space_or_quotes(t->value[pi->i]) && t->value[pi->i] != '\0'
			&& t->value[pi->i] != '*' && t->value[pi->i] != '$')
			pi->i++;
		pi->cur_end = pi->i - 1;
		if (pi->cur_end < pi->i - 1)
		{
			if (t->value[pi->i] != '\0')
				pi->i++;
		}
		new_piece(t, pi, NO_QUOTES);
	}
}

void	mixed_value_assign(t_token *t)
{
	t_pieces_internal	pi;

	pi.i = 0;
	initialize_pieces(&pi);
	while (t->value[pi.i] != '\0')
		handle_piece(t, &pi);
}

void	new_piece(t_token *t, t_pieces_internal *pi, t_quotes_status q)
{
	t_piece	*p;
	int		len;

	p = malloc(sizeof(t_piece));
	if (!p)
		return ;
	len = pi->cur_end - pi->cur_start + 1;
	p->text = malloc(sizeof(char) * (len + 1));
	if (!p->text)
	{
		free(p);
		return ;
	}
	ft_memcpy(p->text, t->value + pi->cur_start, len);
	p->text[len] = '\0';
	p->quotes = q;
	p->next = NULL;
	p->has_tilde = 0;
	p->has_wild = 0;
	p->has_env_v = 0;
	decide_on_extra(p);
	push_piece(t, p);
}

void	push_piece(t_token *t, t_piece *p)
{
	if (!t || !p)
		return ;
	if (!t->pieces)
	{
		t->pieces = p;
		t->pieces_tail = p;
		return ;
	}
	t->pieces_tail->next = p;
	t->pieces_tail = p;
	p->next = NULL;
}
