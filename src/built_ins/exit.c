/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 19:33:37 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/07 03:12:56 by ybutkov          ###   ########.fr       */
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

static int	is_good_numeric(char *string)
{
	int	i;

	i = 0;
	if (string[i] == '+' || string[i] == '-')
		i++;
	if (string[i] == '\0')
		return (0);
	while(string[i])
	{
		if (!ft_isdigit(string[i]))
			return (0);
		i++;
	}
	return (1);
}

int	bi_exit(t_shell *shell, char **args)
{
	printf("exit\n"); // for some reason it appears in every case
	if (args[2] != NULL)
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	shell->free(shell);
	if (!args[1])
		exit(0);
	if (!is_good_numeric(args[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		exit(1);
	}
	else
	// check for atoi overflows?
		exit(ft_atoi(args[1]));
}
