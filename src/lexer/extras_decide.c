/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras_decide.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:01:45 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/22 19:04:54 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

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
		if (p->text[i] == '~' && p->quotes == NO_QUOTES)
			small_tilde_check(p);
		if (p->text[i] == '*' && p->quotes == NO_QUOTES)
			p->has_wild = 1;
		if (p->text[i] == '$')
			small_env_var_check(p);
		i++;
	}
}

void	small_tilde_check(t_piece *p)
{
	int	i;

	if (p->text[0] == '~' && (p->text[1] == '\0' || p->text[1] == '/'))
	{
		p->has_tilde = 1;
		return ;
	}
	i = 1;
	while (p->text[i])
	{
		if (p->text[i] == '~' && (p->text[i - 1] == ':'
				|| p->text[i - 1] == '='))
		{
			if (p->text[i + 1] == '\0' || p->text[i + 1] == '/')
			{
				p->has_tilde = 1;
				return ;
			}
		}
		i++;
	}
}

void	small_env_var_check(t_piece *p)
{
	int	i;

	if (!p || p->text[0] != '$')
		return ;
	if (!ft_isalpha(p->text[1]) && p->text[1] != '_'
		&& p->text[1] != '?' && p->text[1] != '\0')
		return ;
	if (p->text[1] == '\0')
	{
		p->has_env_v = 1;
		return ;
	}
	if (p->text[1] == '?')
	{
		p->has_env_v = 1;
		return ;
	}
	i = 2;
	while (p->text[i])
	{
		if (!ft_isalnum(p->text[i]) && p->text[i] != '_')
			return ;
		i++;
	}
	p->has_env_v = 1;
}

void	decide_on_extra_in_token(t_token *t)
{
	t_piece	*node;

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
