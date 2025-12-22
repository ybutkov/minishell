/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:50:42 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/22 23:16:35 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	heredoc_newline_par_wild(t_token *tok)
{
	if (ft_strcmp(tok->value, "<<") == 0)
		tok->type = TOKEN_HEREDOC;
	else if (ft_strcmp(tok->value, "\n") == 0)
		tok->type = TOKEN_NEWLINE;
	else if (ft_strcmp(tok->value, "(") == 0)
		tok->type = TOKEN_LEFT_PAREN;
	else if (ft_strcmp(tok->value, ")") == 0)
		tok->type = TOKEN_RIGHT_PAREN;
	else if (cust_strchr('*', tok->value))
		tok->type = TOKEN_WILDCARD;
}

void	type_of_token(t_token *tok)
{
	if (tok->stat != NO_QUOTES)
	{
		tok->type = TOKEN_WORD;
		return ;
	}
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
	else if (tok->type == TOKEN_WORD)
		heredoc_newline_par_wild(tok);
	else
		tok->type = TOKEN_WORD;
}

void	end_token(t_lex_inf *lex)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return ;
	init_token(tok, TOKEN_END);
	if (lex->tail)
		lex->tail->next = tok;
	tok->prev = lex->tail;
	lex->tail = tok;
}

int	is_whitespace_or_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&'
		|| c == ';' || (c >= 9 && c <= 12) || (c == 32)
		|| c == '\0');
}
