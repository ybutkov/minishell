/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixed_value_assign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:45:44 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/06 20:30:18 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	mixed_value_assign(t_lex_inf *l, t_token *t)
{
	t_pieces_internal	pi;

	pi.i = 0;
	initialize_pieces(&pi);
	while (t->value[pi.i] != '\0')
	{
		if (t->value[pi.i] == '"')
		assign_quoted_pieces('"', l, t, &pi);
		else if (t->value[pi.i] == '\'')
		assign_quoted_pieces('\'', l, t, &pi);
		else
		{
			pi.cur_start = pi.i;
			while (is_whitespace(t->value[pi.i]))
				pi.i++;
			while (!is_space_or_quotes(t->value[pi.i]) && t->value[pi.i] != '\0')
				pi.i++;
			pi.cur_end = pi.i - 1;
			if (pi.cur_end < pi.i - 1)
			{
				if (t->value[pi.i] != '\0')
					pi.i++;
			}
			new_piece(t, &pi, l, NO_QUOTES);
		}
		// else
		// 	pi.i++;
	}
}

void	assign_quoted_pieces(char quote, t_lex_inf *l, t_token *t, t_pieces_internal *pi)
{
	pi->i++;
	if (t->value[pi->i] == quote)
	{
		pi->i++;
		return ;
	}
	pi->cur_start = pi->i;
	if (quote == '"')
	{
		while (t->value[pi->i] != quote && t->value[pi->i] != '$'
				&& t->value[pi->i] != '\0')
			pi->i++;
	}
	else if (quote == '\'')
	{
		while (t->value[pi->i] != quote
				&& t->value[pi->i] != '\0')
			pi->i++;
	}
	pi->cur_end = pi->i - 1;
	if (quote == '\'')
		new_piece(t, pi, l, SINGLE_Q);
	else if (quote == '"')
		new_piece(t, pi, l, DOUBLE_Q);
	pi->i++;
}

void	new_piece(t_token *t, t_pieces_internal *pi, t_lex_inf *lex, e_quotes_status q)
{
	t_piece	*p;
	int		len;

	p = malloc(sizeof(t_piece));
	if (!p)
	{
		lex->error_code = 2;
		return ;
	}
	len = pi->cur_end - pi->cur_start + 1;
	p->text = malloc(sizeof(char) * (len + 1));
	if (!p->text)
	{
		free(p);
		lex->error_code = 2;
		return ;
	}
	ft_memcpy(p->text, t->value + pi->cur_start, len);
	p->text[len] = '\0';
	p->quotes = q;
	p->next = NULL;
	decide_on_extra(p);
	push_piece(t, p);
}

void	decide_on_extra(t_piece *p)
{
	int	i;
	
	if (!p || !p->text)
		return ;
	i = 0;
	p->has_wild = 0;
	p->has_env_v = 0;
	if (p->quotes == SINGLE_Q)
		return ;
	while (p->text[i])
	{
		if (p->text[i] == '*' && p->quotes == 1)
			p->has_wild = 1;
		if (p->text[i] == '$') //don't know if there are any other specific conditions yet
			p->has_env_v = 1;
		i++;
	}
}

void	decide_on_extra_in_token(t_token *t)
{
	t_piece	*node;
	int		i;
	
	if (t->pieces)
	{
		node = t->pieces;
		while (node)
		{
			if (node->has_wild)
				t->has_wild = 1;
			if (node->has_env_v)
				t->has_env_v = 1;
			node = node->next;
		}
	}
	else if (t->stat == NO_QUOTES || t->stat == DOUBLE_Q)
		extras_token(t);
}

void	extras_token(t_token *t)
{
	int	i;
	
	i = 0;
	while (t->value[i])
	{
		if (t->value[i] == '*' && t->stat == NO_QUOTES)
			t->has_wild = 1;
		if (t->value[i] == '$')
			t->has_env_v = 1;
		i++;		
	}
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
