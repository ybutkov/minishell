/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:38:35 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/05 15:34:57 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "get_next_line.h"

t_token	*read_and_lexicalize()
{
	char    *line;
	t_token *tokens;

	while (1)
	{
		tokens = NULL;
		if (isatty(fileno(stdin)))
			line = readline("$> ");
		else
		{
			char *tmp;
			tmp = get_next_line(fileno(stdin));
			if (!tmp)
				return (NULL);
			line = ft_strtrim(tmp, "\n");
			free(tmp);
		}
		if (!line)
			return (NULL);
		if (*line == '\0')
		{
			free(line);
			if (isatty(fileno(stdin)))
				continue ;
			else
				continue ;
		}
		tokens = lexicalization(line);
		if (tokens)
			add_history(line);
		free(line);
		return (tokens);
	}
}
