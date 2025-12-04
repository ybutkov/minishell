/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:26:13 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/02 22:50:10 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

// create a token
// append a token
// free the whole token thing
// recognize a token type

void	new_token(t_lex_inf *lex, e_quotes_status status)
{
	t_token *tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
	{
		lex->error_code = 2;
		return ;
	}
	token_init(tok);
	tok->stat = status;
	if (tok->stat == MIXED)
	{
		simple_value(lex, tok);
		mixed_value_assign(lex, tok);
	}
	else
		simple_value(lex, tok);
	type_of_token(tok);
	push_token(lex, tok);
}
	
void	simple_value(t_lex_inf *lex, t_token *tok)
{
	int		len;

	len = lex->end - lex->start + 1;
	tok->value = malloc(sizeof(char) * len + 1);
	if (!tok->value)
	{
		free(tok);
		lex->error_code = 2;
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
	else if (cust_strchr('*', tok->value))
		tok->type = TOKEN_WILDCARD;
	else
		tok->type = TOKEN_WORD;
}

void	end_token(t_lex_inf *lex)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
	{
		lex->error_code = 2;
		return ;
	}
	tok->type = TOKEN_END;
	tok->value = NULL;
	if(lex->tail) 
		lex->tail->next = tok;
	tok->prev = lex->tail;
	lex->tail = tok;
	tok->next = NULL;
}

