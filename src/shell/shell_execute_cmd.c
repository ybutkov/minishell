/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:57:40 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 21:56:42 by ybutkov          ###   ########.fr       */
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
#include <sys/types.h>
#include <dirent.h>

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

int	collect_tokens_in_cmd(t_shell *shell, t_list **arg_list,
	t_token *start_tkn)
{
	t_token	*curr_tkn;

	curr_tkn = start_tkn;
	while (curr_tkn && curr_tkn->type != TOKEN_END)
	{
		if (collect_pieces_to_strings(shell, curr_tkn, arg_list) == NO)
		{
			ft_lstclear(arg_list, free);
			return (free(*arg_list), NO);
		}
		curr_tkn = curr_tkn->next;
	}
	return (OK);
}

int	collect_argv(t_shell *shell, t_cmd *cmd)
{
	t_list	*arg_list;

	arg_list = NULL;
	if (collect_tokens_in_cmd(shell, &arg_list, cmd->tokens) == NO)
		return (ft_lstclear(&arg_list, empty_func), NO);
	cmd->argv = list_to_array(arg_list);
	ft_lstclear(&arg_list, empty_func);
	if (cmd->argv[0] && cmd->argv[0][0] != '\0')
	{
		cmd->path = get_cmd_path(shell->ctx->env, cmd->argv[0]);
		if (!cmd->path)
			cmd->path = ft_strdup(cmd->argv[0]);
	}
	ft_lstclear(&arg_list, empty_func);
	return (OK);
}

void	validate_argv(t_cmd *cmd, t_shell *shell)
{
	if (cmd->argv == NULL)
	{
		shell->free(shell);
		exit(EXIT_SUCCESS);
	}
	if (opendir(cmd->argv[0]) != NULL)
		output_error_and_exit(cmd->argv[0], MSG_IS_DIRECTORY, shell,
			EXIT_CMD_CANNOT_EXEC);
	if (!cmd->path || access(cmd->path, X_OK) != 0)
		output_error_and_exit(cmd->argv[0], CMD_NOT_FOUND_MSG, shell,
			EXIT_CMD_NOT_FOUND);
}

int	execute_single_in_fork(t_cmd *cmd, t_shell *shell, int in_fd,
		int out_fd)
{
	pid_t	pid;
	int		status;

	set_signals_waiting_parent();
	pid = fork();
	if (pid < 0)
	{
		output_error(FORK_ERROR, NULL);
		return (EXIT_FAILURE_CREATE_FORK);
	}
	else if (pid == 0)
	{
		set_signals_child();
		dup2_and_close_both(in_fd, out_fd);
		apply_redirect(cmd, shell);
		validate_argv(cmd, shell);
		execve(cmd->path, cmd->argv,
			shell->ctx->env->to_array(shell->ctx->env));
		shell->free(shell);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	return (return_status(status));
}

int	execute_cmd(t_cmd *cmd, t_shell *shell, int input_fd, int output_fd)
{
	int	bi_func;

	if (collect_argv(shell, cmd) == NO)
	{
		shell->free(shell);
		exit(EXIT_FAILURE);
	}
	if (cmd->argv)
	{
		bi_func = builtin_func(cmd->argv[0]);
		if (bi_func != -1)
		{

			return (builtin(bi_func, cmd, shell, input_fd, output_fd));
		}
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
	// Always in fork to avoid replacing the shell process ??
	return (execute_single_in_fork(cmd, shell, input_fd, output_fd));
}
