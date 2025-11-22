/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:26:13 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/21 16:10:00 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

// create a token
// append a token
// free the whole token thing
// recognize a token type

	


t_token	*new_token(t_lex_inf *lex)
{
	t_token *tok;
	int		len;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	len = lex->end - lex->start + 1;
	if (tok->stat == MIXED)
		mixed_value_assign(&tok, &lex);
	else
	{
		tok->value = malloc(sizeof(char) * len + 1);
		if (!tok->value)
		{
			free(tok);
			return (NULL);
		}
		ft_memcpy(tok->value, lex->line + lex->start, len);
		tok->value[len] = '\0';
	}
	type_of_token(tok);
	tok->prev = NULL;
	tok->next = NULL;
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

void	mixed_value_assign(t_token *tok, t_lex_inf *lex)
{
	int	i;

	while (i < lex->end)
	{
		//here we could have a linked list of all the
		//parts of the node as a linked list, each part having the 
		//status of quotation to it
	}
}

void	type_of_token(t_token *tok)
{
	if (ft_strcmp(tok->value, "&&") == 0)
		tok->type = TOKEN_AND;
	else if (ft_strcmp(tok->value, "||") == 0)
		tok->type = TOKEN_OR;
	else if (ft_strcmp(tok->value, ";") == 0)
		tok->type = TOKEN_SEMICOLON;
	else if (ft_strcmp(tok->value, "|") == 0)
		tok->type = TOKEN_PIPE;
	else if (ft_strcmp(tok->value, "<") == 0)
		tok->type = TOKEN_REDIR_IN;
	else if (ft_strcmp(tok->value, ">") == 0)
		tok->type = TOKEN_REDIR_OUT;
	else if (ft_strcmp(tok->value, ">>") == 0)
		tok->type = TOKEN_REDIR_APPEND;
	else if (ft_strcmp(tok->value, "<<") == 0)
		tok->type = TOKEN_HEREDOC;
	else if (ft_strcmp(tok->value, "\n") == 0)
		tok->type = TOKEN_NEWLINE;
	else
		tok->type = TOKEN_WORD;
}

