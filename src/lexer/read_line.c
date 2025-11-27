/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:38:35 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/27 16:19:45 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

t_token	*read_and_lexicalize()
{
	char	*line;
	size_t	line_len;
	t_token	*tokens;
	ssize_t	n;

	line = NULL;
	line_len = 0;
	tokens = NULL;

	write(1, "$> ", 3);
	n = getline(&line, &line_len, stdin);
	if (n == -1)
	{
		free(line);
		return (tokens);
	}
	if (n > 0 && line[n - 1] == '\n')
        line[n - 1] = '\0';
	tokens = lexicalization(line);
	return (tokens);
}