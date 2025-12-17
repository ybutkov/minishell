/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 19:33:37 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/17 19:18:42 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//either user types exit: then write "exit" and exit, haha -
//or the programm incounters a fatal internal error in the process

//all the error codes and their sources here

// 0: Success (standard EXIT_SUCCESS). Used across the code (e.g., shell_execute.c, heredoc child).
// 1: General failure / internal error (EXIT_FAILURE, and project-specific EXIT_FAILURE_CREATE_PIPE / EXIT_FAILURE_CREATE_FORK are defined as 1 in error.h).
// 127: Command not found. The project defines #define EXIT_CMD_NOT_FOUND 127 in constants.h and uses it when an exec fails to find a command.
// 126: Command found but not executable (permission problem). Common shell convention — implement if you want consistent behavior with POSIX shells.
// 128 + N: Child terminated by signal N. Shells set the exit status to 128 + signal number (examples: SIGINT → 130, SIGQUIT → 131). Your shell should follow this when reporting child exit status (use WIFSIGNALED/WTERMSIG on wait status).
// 255 (convention): Invalid numeric argument to exit (many shells use 255 for "numeric argument required"). If the exit builtin is given a non-numeric argument you should print an error and choose a code (255 is common).

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
	// check for atoi overflows?
	exit_status = ft_atoi(args[1]);
	shell->ctx->should_exit = 1;
	return (exit_status % 256);
}
