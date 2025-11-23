/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol_identifier.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:35:24 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/23 22:28:17 by ashadrin         ###   ########.fr       */
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
			|| c == ';' || (c >= 9 && c<= 12) || (c == 32));
}

int	is_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&' || c == ';');
}

int	is_space_or_quotes(char c)
{
	return (c == '\'' || c == '"' || (c >= 9 && c<= 12) || (c == 32));
}
