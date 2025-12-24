/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_heredoc_expand.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:20:16 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 02:20:20 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "parsing.h"
#include "shell.h"
#include "shell_internal.h"
#include "shell_utils.h"
#include "signals.h"
#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

static int	append_and_free(char **res, char *append_str)
{
	ft_strappend(res, append_str);
	free(append_str);
	return (OK);
}

static int	expand_var_name(char *line, int *i, char **res, t_ctx *ctx)
{
	int		start;
	char	*name;

	(*i)++;
	start = *i;
	if ((line)[*i] == '?')
	{
		(*i)++;
		append_and_free(res, ft_itoa(ctx->last_exit_status));
		return (0);
	}
	while (!will_end_env_var(line[*i]))
		(*i)++;
	name = ft_substr(line, start, (size_t)(*i - start));
	ft_strappend(res, ctx->env->get_value(ctx->env, name));
	free(name);
	return (1);
}

int	expand_heredoc_line(char **line, t_shell *shell)
{
	char	*res;
	int		i;
	int		start;

	i = 0;
	start = 0;
	res = NULL;
	while ((*line)[i])
	{
		if ((*line)[i] == '$')
		{
			append_and_free(&res, ft_substr((*line), start, i - start));
			expand_var_name(*line, &i, &res, shell->ctx);
			start = i;
		}
		i++;
	}
	if (start < i)
		append_and_free(&res, ft_substr((*line), start, i - start));
	free(*line);
	*line = res;
	return (OK);
}
