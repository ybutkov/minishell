/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 11:26:58 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/04 15:44:09 by ashadrin         ###   ########.fr       */
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
		else if (is_special(&lex))
			is_operator(&lex);
		else if (line[lex.i] == '\'')
			is_single_quote(&lex);
		else if (line[lex.i] == '"')
			is_double_quote(&lex);
		else if (line[lex.i] == '#')
			break ;
		else if (line[0])
			is_space(&lex);
	}
	end_token(&lex);
	return (lex.head);
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
