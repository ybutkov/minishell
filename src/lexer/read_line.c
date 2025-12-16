/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:38:35 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/16 13:19:38 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "get_next_line.h"
#include "signals.h"

t_token	*read_and_lexicalize()
{
	char    *line;
	char 	*tmp;
	t_token *tokens;

	disable_ctrl_echo();
	set_signals_parent_interactive();
	while (1)
	{
		tokens = NULL;
		if (isatty(fileno(stdin)))
			line = readline("$> ");
		else
		{
			tmp = get_next_line(fileno(stdin));
			if (!tmp)
				return (NULL);
			// line = ft_strtrim(tmp, "\n");
			// free(tmp);
			line = tmp;
			// if (line)
			// {
			// 	len = ft_strlen(line);
			// 	if (len > 0 && line[len - 1] == '\n')
			// 	line[len - 1] = '\0';
			// }
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
		if (line)
			add_history(line);
		line = preprocessing(line);
		tokens = lexicalization(line);
		// printf("%s\n", line);
		free(line);
		return (tokens);
	}
}
