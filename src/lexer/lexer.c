/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 11:26:58 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/21 16:19:41 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

// space, newline, tab - split tokens outside quotes;
// single quotes - everything inside literal;
// double quotes - everything literal, except for $, \, and `;
// \ - cancelling the meaning of the next sign, but not erasing it;
// # all the rest of the line is commented out;

t_token	*lexicalization(char *line)
{
	t_lex_inf	lex;

	lex_struct_init(line, &lex);
	while (line[lex.i])
	{
		if (is_whitespace(line[lex.i]))
			is_space(&lex);
		else if (line[lex.i] == '\'')
			is_single_quote(&lex);
		else if (line[lex.i] == '"')
			is_double_quote(&lex);
		else if (line[lex.i] == '#')
			break ;
		else if (line[0])
			is_space(&lex);
	}
}

void	is_space(t_lex_inf *lex)
{
	int	quotes;

	quotes == 0;
	while (lex->line[lex->i] == ' ')
		lex->i++;
	if ((lex->line[lex->i] == '"') || (lex->line[lex->i] == '\''))
		return ;
	lex->start = lex->i;
	while (lex->line[lex->i] != '\0' && lex->line[lex->i] != ' ')
	{
		if (lex->line[lex->i] == '"')
		{
			quotes_within(lex, '"');
			quotes = 1;
		}
		else if (lex->line[lex->i] != '\'')
		{
			quotes_within(lex, '\'');
			quotes = 1;
		}
	}	
	lex->i++;
	lex->end = lex->i - 1;
	if (quotes == 0)
		new_token(&lex);
}

void	quotes_within(t_lex_inf *lex, char quotes)
{
	lex->i++;
	lex->error_code = 1;
	while (lex->line[lex->i] != quotes && lex->line[lex->i] != '\0')
		lex->i++;
	if (lex->line[lex->i] == quotes)
	{
		lex->error_code = 0;
		while (!is_whitespace(lex->line[lex->i])
			&& lex->line[lex->i] != '\0')
			// i should probably look for further
			//unclosed quotes here as well, but not in terms
			//of creating new tokens - just checking whether 
			//they're closing
			lex->i++;
		lex->end = lex->i - 1;
		new_token(&lex);
	}
}


void	is_single_quote(t_lex_inf *lex)
{
	lex->error_code = 1;
	lex->i++;
	lex->start = lex->i;
	while (lex->line[lex->i] != '\'' || lex->line[lex->i] != '\0')
		lex->i++;
	if (lex->line[lex->i] == '\'')
	{
		lex->error_code = 0;
		lex->end = lex->i - 1;
		new_token(&lex);
	}
}

void	is_double_quote(t_lex_inf *lex)
{
	lex->error_code = 1;
	lex->i++;
	lex->start = lex->i;
	while (lex->line[lex->i] != '"' || lex->line[lex->i] != '\0')
		lex->i++;
	if (lex->line[lex->i] == '"')
	{
		lex->error_code = 0;
		lex->end = lex->i - 1;
		new_token(&lex);
	}
}

void lex_struct_init(char *line, t_lex_inf *lex)
{
	lex->line = line;
	lex->i = 0;
	lex->start = 0;
	lex->end = 0;
	lex->head = NULL;
	lex->tail = NULL;
	lex->error_code = 0;
}

