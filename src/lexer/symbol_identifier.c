/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol_identifier.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:35:24 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/15 14:18:57 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

int	is_whitespace(char c)
{
	return ((c >= 9 && c<= 12) || (c == 32));
}

int	is_whitespace_or_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&'
			|| c == ';' || (c >= 9 && c<= 12) || (c == 32)
			|| c == '\0');
}
// && || ; \ < > >> << ( ) 
int	is_special(t_lex_inf *lex)
{
	char	c;
	
	c = lex->line[lex->i];
	if (c == ';' || c == '(' || c == ')' || c == '<'
		|| c == '>' || c == '|')
		return (1);
	if (c == '&')
	{
		if (lex->line[lex->i + 1] == c)
			return (1);
	}
	return (0);
	
}

int	is_space_or_quotes(char c)
{
	return (c == '\'' || c == '"' || (c >= 9 && c<= 12) || (c == 32));
}

int	will_end_env_var(char c)
{
	return (c == ':' || c == '=' || c == '/' || c == '*' || c == '$'
				|| is_space_or_quotes(c) || c == '\0');
}