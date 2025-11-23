/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:26:13 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/22 18:15:02 by ashadrin         ###   ########.fr       */
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
		return (NULL);
	tok->stat = status;
	if (tok->stat == MIXED)
		mixed_value_assign(lex, tok);
	else
		not_mixed(lex, tok);
	type_of_token(tok);
	tok->prev = NULL;
	tok->next = NULL;
	push_token(lex, tok);
}
	
void	not_mixed(t_lex_inf *lex, t_token *tok)
{
	int		len;

	len = lex->end - lex->start + 1;
	tok->value = malloc(sizeof(char) * len + 1);
	if (!tok->value)
	{
		free(tok);
		return (NULL);
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

void	mixed_value_assign(t_lex_inf *lex, t_token *tok)
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

// void	assign_stat(t_token *tok, int status)
// {
// 	if (status == 1)
// 		tok->stat = NO_QUOTES;
// 	else if (status == 2)
// 		tok->stat = SINGLE_Q;
// 	else if (status == 3)
// 		tok->stat = DOUBLE_Q;
// 	else if (status = 4)
// 		tok->stat = MIXED;
// }
