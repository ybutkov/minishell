/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:38:35 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 22:36:49 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "get_next_line.h"
#include "signals.h"

char	*get_the_line(void)
{
	char	*line;
	char	*tmp;
	int		len;

	if (isatty(fileno(stdin)))
		line = readline("$> ");
	else
	{
		tmp = get_next_line(fileno(stdin));
		if (!tmp)
			return (NULL);
		line = tmp;
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
	}
	return (line);
}

t_token	*read_and_lexicalize(void)
{
	char	*line;
	t_token	*tokens;

	disable_ctrl_echo();
	set_signals_parent_interactive();
	while (1)
	{
		tokens = NULL;
		line = get_the_line();
		if (!line)
			return (NULL);
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		tokens = lexicalization(line);
		free(line);
		if (tokens == NULL)
			continue ;
		return (tokens);
	}
}
