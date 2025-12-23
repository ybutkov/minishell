/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 19:33:37 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/23 23:32:09 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include "error.h"

static int	is_good_numeric(char *string)
{
	int	i;

	i = 0;
	if (string[i] == '+' || string[i] == '-')
		i++;
	if (string[i] == '\0')
		return (0);
	while (string[i])
	{
		if (!ft_isdigit(string[i]))
			return (0);
		i++;
	}
	return (1);
}

int	bi_exit(t_shell *shell, char **args)
{
	int		exit_status;
	char	*err_msg;

	if (isatty(STDIN_FILENO))
		write(1, MSG_EXIT_NL, ft_strlen(MSG_EXIT_NL));
	if (!args[1])
	{
		shell->ctx->should_exit = 1;
		return (EXIT_SUCCESS);
	}
	if (args[1] && is_good_numeric(args[1]) && args[2])
	{
		output_error(MSG_EXIT, MSG_TOO_MANY_ARG);
		return (EXIT_FAILURE);
	}
	if (!is_good_numeric(args[1]))
	{
		err_msg = ft_strjoin(MSG_EXIT_2_COLON, args[1]);
		output_error(err_msg, EXIT_MSG_NUMBER_REQUIRED);
		free(err_msg);
		shell->ctx->should_exit = 1;
		return (EXIT_MISUSE);
	}
	exit_status = ft_atoi(args[1]);
	return (shell->ctx->should_exit = 1, exit_status % 256);
}
