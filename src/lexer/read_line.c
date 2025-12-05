/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:38:35 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/02 18:34:19 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

t_token	*read_and_lexicalize()
{
	char    *line;
	t_token *tokens;

	tokens = NULL;
	line = readline("$> ");
	if (!line)
		return (tokens);
	else
		add_history(line);
	tokens = lexicalization(line);
	free(line);
	return (tokens);
}