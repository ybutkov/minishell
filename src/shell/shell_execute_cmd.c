/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:57:40 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/18 16:20:19 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_internal.h"
#include "constants.h"
#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "shell.h"
#include "utils.h"
#include "builtin.h"
#include "shell_utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "signals.h"

char	**get_built_in_list(void)
{
	static char	*builtins[8];
	static int	initialized;

	if (!initialized)
	{
		builtins[0] = "echo";
		builtins[1] = "cd";
		builtins[2] = "pwd";
		builtins[3] = "export";
		builtins[4] = "unset";
		builtins[5] = "env";
		builtins[6] = "exit";
		builtins[7] = NULL;
		initialized = 1;
	}
	return (builtins);
}

int	builtin_func(char *command)
{
	char	**built_ins;
	int		i;

	if (!command)
		return (-1);
	built_ins = get_built_in_list();
	i = 0;
	while (built_ins[i])
	{
		if (ft_strcmp(command, built_ins[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	execute_single_in_fork(t_cmd *cmd, t_shell *shell, int input_fd,
		int output_fd)
{
	pid_t	pid;
	int		status;

	set_signals_waiting_parent();
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		set_signals_child();
		if (input_fd != STDIN_FILENO)
			dup2_and_close(input_fd, STDIN_FILENO);
		if (output_fd != STDOUT_FILENO)
			dup2_and_close(output_fd, STDOUT_FILENO);
		else
			dup2(STDOUT_FILENO, STDOUT_FILENO);
		apply_redirect(cmd, shell);
		if (cmd->argv == NULL)
		{
			shell->free(shell);
			exit(EXIT_SUCCESS);
		}
		if (!cmd->path || access(cmd->path, X_OK) != 0)
			output_error_and_exit(cmd->argv[0], CMD_NOT_FOUND_MSG, shell,
				EXIT_CMD_NOT_FOUND);
		execve(cmd->path, cmd->argv, shell->ctx->env->to_array(shell->ctx->env));
		shell->free(shell);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}

int	execute_cmd(t_cmd *cmd, t_shell *shell, int input_fd, int output_fd)
{
	int	bi_func;

	if (cmd->argv)
	{
		bi_func = builtin_func(cmd->argv[0]);
		if (bi_func != -1)
			return (builtin(bi_func, cmd, shell, input_fd, output_fd));
	}
	// check for single command. execute in sep fork
	// if (input_fd == STDIN_FILENO && output_fd == STDOUT_FILENO)
	// 	return (execute_single_in_fork(cmd, shell, input_fd, output_fd));
	// // apply_heredoc(cmd, shell);
	// if (input_fd != STDIN_FILENO)
	// {
	// 	dup2(input_fd, STDIN_FILENO);
	// 	close(input_fd);
	// }
	// if (output_fd != STDOUT_FILENO)
	// {
	// 	dup2(output_fd, STDOUT_FILENO);
	// 	close(output_fd);
	// }
	// apply_redirect(cmd, shell);
	// if (!cmd->path || access(cmd->path, X_OK) != 0)
	// 	output_error_and_exit(cmd->argv[0], CMD_NOT_FOUND_MSG, shell,
	// 		EXIT_CMD_NOT_FOUND);
	// execve(cmd->path, cmd->argv, shell->ctx->envp);
	// shell->free(shell);
	// exit(EXIT_FAILURE);
	// Always execute external commands in a fork to avoid replacing the shell process ??
	return (execute_single_in_fork(cmd, shell, input_fd, output_fd));
}
