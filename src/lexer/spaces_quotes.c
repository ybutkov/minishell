/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:22:09 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/14 23:49:43 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	is_space(t_lex_inf *lex)
{
	while (is_whitespace(lex->line[lex->i]))
		lex->i++;
	if ((lex->line[lex->i] == '"') || (lex->line[lex->i] == '\'') || is_special(lex))
		return ;
	lex->start = lex->i;
	while (lex->line[lex->i] != '\0' && !is_whitespace(lex->line[lex->i])
			&& !is_special(lex))
	{
		if (lex->line[lex->i] == '\'' || lex->line[lex->i] == '"')
		{
			quotes_within(lex);
			return ;
		}
		lex->i++;
	}	
	lex->end = lex->i - 1;
	if (lex->end >= lex->start)
		new_token(lex, NO_QUOTES);
}

void	quotes_within(t_lex_inf *l)
{
	e_mix	quo_closed;
	char	quotes;

	quo_closed = ALL_CLOSED;
	while (l->line[l->i] != '\0')
	{
		decide_on_quotes(l, &quo_closed, &quotes);
		if (quo_closed == 0 && is_whitespace_or_special(l->line[l->i + 1]))
		{
			l->end = l->i;
			new_token(l, MIXED);
			l->i = l->end + 1;
			return ;
		}
		l->i++;
	} 
}
// ckkshd"nclmc klxld"mcldl'cockelpps x x cdcf 'jofijo4f

void	is_single_quote(t_lex_inf *lex)
{
	lex->error_code = 1;
	lex->i++;
	lex->start = lex->i;
	while (lex->line[lex->i] != '\'' && lex->line[lex->i] != '\0')
		lex->i++;
	if (lex->line[lex->i] == '\'')
	{
		lex->error_code = 0;
		if (!is_whitespace_or_special(lex->line[lex->i + 1]))
		{
			lex->i = lex->start - 1;
			quotes_within(lex);
			return ;
		}
        lex->end = lex->i - 1;
        new_token(lex, SINGLE_Q);
        lex->i++;    
		// if (is_whitespace(lex->line[lex->i + 1]) || is_special(lex))
		// {
		// 	lex->error_code = 0;
		// 	lex->end = lex->i - 1;
		// 	new_token(lex, SINGLE_Q);
		// 	lex->i++;
		// }
		// else 
		// {
		// 	lex->start--;
		// 	lex->i = lex->start;
		// 	quotes_within(lex);
		// }
	}
}

void	is_double_quote(t_lex_inf *lex)
{
	lex->error_code = 1;
	lex->i++;
	lex->start = lex->i;
	while (lex->line[lex->i] != '"' && lex->line[lex->i] != '\0')
		lex->i++;
	if (lex->line[lex->i] == '"')
	{
		lex->error_code = 0;
        lex->end = lex->i - 1;
        new_token(lex, DOUBLE_Q);
        lex->i++;    
		// if (is_whitespace(lex->line[lex->i + 1]) || is_special(lex))
		// {
		// 	lex->error_code = 0;
		// 	lex->end = lex->i - 1;
		// 	new_token(lex, DOUBLE_Q);
		// 	lex->i++;
		// }
		// else 
		// {
		// 	lex->start--;
		// 	lex->i = lex->start;
		// 	quotes_within(lex);
		// }
	}
}
// && || : \ < > >> << ( ) 
void	is_operator(t_lex_inf *lex)
{
	char sym;
	
	lex->start = lex->i;
	sym = lex->line[lex->i];
	if ((sym == '/' || sym == '&' || sym == '|' || sym == '>'
			|| sym == '<') && (sym == lex->line[lex->i + 1]))
	{
		lex->i++;
		lex->end = lex->i;
		new_token(lex, NO_QUOTES);
		lex->i++;
		return ;
	}
	lex->end = lex->i;
	new_token(lex, NO_QUOTES);
	lex->i++;
}

// // void	quotes_within(t_lex_inf *l)
// // {
// 	e_mix	quo_closed;
// 	char	quotes;

// 	// printf("Type: %d\n",(int)quo_closed);
// 	quo_closed = ALL_CLOSED;
// 	while (l->line[l->i] != '\0')
// 	{
// 		decide_on_quotes(l, &quo_closed, &quotes);
// 		if (quo_closed == 0 && is_whitespace_or_special(l->line[l->i + 1]))
// 		{
// 			l->end = l->i;
// 			new_token(l, MIXED);
// 			l->i = l->end + 1;
// 			return ;
// 		}
// 		l->i++;
// 	} 
// }
// // ckkshd"nclmc klxld"mcldl'cockelpps x x cdcf 'jofijo4f

// void	is_single_quote(t_lex_inf *lex)
// {
// 	lex->error_code = 1;
// 	lex->i++;
// 	lex->start = lex->i;
// 	while (lex->line[lex->i] != '\'' && lex->line[lex->i] != '\0')
// 		lex->i++;
// 	if (lex->line[lex->i] == '\'')
// 	{
// 		lex->error_code = 0;
// 		lex->end = lex->i - 1;
// 		new_token(lex, SINGLE_Q);
// 		lex->i++;
// 	}
// }

// void	is_double_quote(t_lex_inf *lex)
// {
// 	lex->error_code = 1;
// 	lex->i++;
// 	lex->start = lex->i;
// 	while (lex->line[lex->i] != '"' && lex->line[lex->i] != '\0')
// 		lex->i++;
// 	if (lex->line[lex->i] == '"')
// 	{
// 		lex->error_code = 0;
// 		lex->end = lex->i - 1;
// 		new_token(lex, DOUBLE_Q);
// 		lex->i++;
// 	}
// }