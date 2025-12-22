/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 22:08:41 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/22 23:22:25 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>
#include "lexer_internal.h"

void	initializing_preproc(t_preproc *p, char *str)
{
	p->res_len = ft_strlen(str);
	p->single_opened = 0;
	p->double_opened = 0;
	p->i = 0;
	p->j = 0;
}

static int	handle_backslash(t_preproc *p, char *str)
{
	if (p->double_opened == 1)
		p->delta_size = 3;
	else
		p->delta_size = 1;
	p->res = (char *)ft_realloc(p->res,
			p->res_len, p->res_len + p->delta_size + 1);
	p->res_len += p->delta_size;
	if (p->res == NULL)
		return (0);
	if (p->double_opened == 1)
		p->res[p->j++] = '"';
	p->res[p->j++] = '\'';
	p->i++;
	p->res[p->j++] = str[p->i++];
	p->res[p->j++] = '\'';
	if (p->double_opened == 1)
		p->res[p->j++] = '"';
	return (1);
}

static int	preproc_loop(t_preproc *p, char *str)
{
	while (str[p->i])
	{
		if (str[p->i] == '\'')
		{
			p->single_opened = !p->single_opened;
			p->res[p->j++] = str[p->i++];
			continue ;
		}
		if (str[p->i] == '"')
		{
			p->double_opened = !p->double_opened;
			p->res[p->j++] = str[p->i++];
			continue ;
		}
		if (str[p->i] == '\\' && p->single_opened == 0)
		{
			if (!handle_backslash(p, str))
				return (0);
			continue ;
		}
		p->res[p->j++] = str[p->i++];
	}
	return (1);
}

char	*preprocessing(char *str)
{
	t_preproc	p;

	initializing_preproc(&p, str);
	p.res = (char *)malloc(sizeof(char) * (p.res_len + 1));
	if (p.res == NULL)
		return (free(str), NULL);
	ft_memset(p.res, 0, (p.res_len + 1));
	if (!preproc_loop(&p, str))
	{
		free(str);
		return (NULL);
	}
	free(str);
	return (p.res);
}
