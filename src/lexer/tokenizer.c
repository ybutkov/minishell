/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:26:13 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 03:53:36 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	new_token(t_lex_inf *lex, t_quotes_status status)
{
	t_token	*tok;

	tok = create_token();
	if (!tok)
		return ;
	tok->stat = status;
	simple_value(lex, tok);
	check_mixed(tok);
	if (tok->stat == MIXED)
		mixed_value_assign(tok);
	decide_on_extra_in_token(tok);
	type_of_token(tok);
	push_token(lex, tok);
}

void	check_mixed(t_token *tok)
{
	int	i;

	i = 0;
	if (tok->stat == NO_QUOTES || tok->stat == DOUBLE_Q)
	{
		while (tok->value[i])
		{
			if (tok->value[i] == '$'
				|| (tok->value[i] == '*' && tok->stat == NO_QUOTES)
				|| (tok->value[i] == '~' && tok->stat == NO_QUOTES))
				tok->stat = MIXED;
			i++;
		}
	}
}

void	simple_value(t_lex_inf *lex, t_token *tok)
{
	int		len;

	len = lex->end - lex->start + 1;
	tok->value = malloc(sizeof(char) * len + 1);
	if (!tok->value)
	{
		free(tok);
		return ;
	}
	ft_memcpy(tok->value, lex->line + lex->start, len);
	tok->value[len] = '\0';
}

void	push_token(t_lex_inf *lex, t_token *tok)
{
	if (!tok || !lex)
		return ;
	if (!lex->head)
	{
		lex->head = tok;
		lex->tail = tok;
		return ;
	}
	lex->tail->next = tok;
	tok->prev = lex->tail;
	lex->tail = tok;
}
