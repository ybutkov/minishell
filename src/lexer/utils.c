/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:32:29 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/02 22:50:13 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

int	cust_strchr(char c, char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	decide_on_quotes(t_lex_inf *l, e_mix *q, char *quotes)
{
	if (l->line[l->i] == '"')
	{
		if (*q == 0)
			*q = 1;
		else if (*q == 1)
			*q = 0;
	}
	if (l->line[l->i] == '\'')
	{
		if (*q == 0)
			*q = 2;
		else if (*q == 2)
			*q = 0;
	}
	*quotes = l->line[l->i]; 
}

void	initialize_pieces(t_pieces_internal *pieces)
{
	pieces->i = 0;
	pieces->cur_start = 0;
	pieces->cur_end = -1;
	pieces->q_stat = ALL_CLOSED;
	pieces->quotes = ' ';
}

void	token_init(t_token *tok)
{
	tok->type = TOKEN_WORD;
	tok->stat = NO_QUOTES;
	tok->value = NULL;
	tok->pieces = NULL;
	tok->pieces_tail = NULL;
	tok->prev = NULL;
	tok->next = NULL;

}