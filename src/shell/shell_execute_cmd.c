/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:57:40 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/22 01:51:06 by ybutkov          ###   ########.fr       */
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

// int	split_string_to_list2(const char *str, t_list **arg_list)
// {
// 	int		i;
// 	int		start;
// 	char	quote;
// 	char	*token;

// 	if (!str || !arg_list)
// 		return (NO);
// 	i = 0;
// 	while (str[i])
// 	{
// 		while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
// 				|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f'))
// 			i++;
// 		if (!str[i])
// 			break ;
// 		if (str[i] == '\'' || str[i] == '"')
// 		{
// 			quote = str[i];
// 			start = i;
// 			i++; /* include opening quote */
// 			while (str[i] && str[i] != quote)
// 				i++;
// 			if (str[i] == quote)
// 				i++; /* include closing quote */
// 			token = ft_substr(str, start, i - start);
// 		}
// 		else
// 		{
// 			start = i;
// 			while (str[i] && !(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
// 						|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f'))
// 			{
// 				/* If we hit an '=' and the next non-space char is a quote,
// 				   include the quoted segment as part of the same token so
// 				   HH = 'ls -la' becomes one token "HH='ls -la'" */
// 				if (str[i] == '=')
// 				{
// 					int j = i + 1;
// 					/* skip spaces between = and opening quote */
// 					while (str[j] && (str[j] == ' ' || str[j] == '\t' || str[j] == '\n'
// 						|| str[j] == '\r' || str[j] == '\v' || str[j] == '\f'))
// 						j++;
// 					if (str[j] == '\'' || str[j] == '"')
// 					{
// 						char q = str[j];
// 						int k = j + 1;
// 						while (str[k] && str[k] != q)
// 							k++;
// 						/* if we found closing quote, move i past it */
// 						if (str[k] == q)
// 							i = k + 1;
// 						else
// 							i = k;
// 						continue ;
// 					}
// 				}
// 				i++;
// 			}
// 			token = ft_substr(str, start, i - start);
// 		}
// 		if (!token)
// 			return (NO);
// 		ft_lstadd_back(arg_list, ft_lstnew(token));
// 	}
// 	return (OK);
// }

// int	rebuild_argv_in_cmd(t_cmd *cmd, t_shell *shell)
// {
// 	t_list	*arg_list;
// 	char	*path;
// 	int		i;

// 	i = 0;
// 	arg_list = NULL;
// 	while (cmd->argv && cmd->argv[i])
// 	{
// 		if (split_string_to_list2(cmd->argv[i], &arg_list) == NO)
// 			return (ft_lstclear(&arg_list, empty_func), NO);
// 		i++;
// 	}
// 	free_str_array(cmd->argv);
// 	cmd->argv = list_to_array(arg_list);
// 	if (cmd->argv[0] && cmd->argv[0][0] != '\0')
// 	{
// 		path = get_cmd_path(shell->ctx->env, cmd->argv[0]);
// 		cmd->path = path;
// 	}
// 	else
// 	{
// 		free_str_array(cmd->argv);
// 		cmd->argv = NULL;
// 	}
// 	ft_lstclear(&arg_list, empty_func);
// 	return (OK);
// }

int	split_str_to_list(char *str, t_list **arg_list)
{
	char	*new_elem;
	int		i;
	int		opened_quote;
	int		prev;

	(void)arg_list;
	i = 0;
	prev = 0;
	opened_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			if (opened_quote)
				opened_quote = !opened_quote;
			i++;
			continue ;
		}
		if (!opened_quote && is_char_space(str[i]))
		{
			new_elem = ft_substr(str, prev, i - prev);
			if (new_elem == NULL)
				return (NO);
			ft_lstadd_back(arg_list, ft_lstnew(new_elem));
			prev = i;
		}
		i++;
	}
	new_elem = ft_substr(str, prev, i - prev);
	if (new_elem == NULL)
		return (NO);
	ft_lstadd_back(arg_list, ft_lstnew(new_elem));
	// free(str);
	return (OK);
}

int	collect_tokens_in_cmd(t_shell *shell, t_list **arg_list,
	t_token *start_tkn)
{
	t_token	*curr_tkn;
	// char	**new_args;

	curr_tkn = start_tkn;
	while (curr_tkn && curr_tkn->type != TOKEN_END)
	{
		// new_args = collect_pieces_to_strings(shell, curr_tkn, arg_list);
		if (collect_pieces_to_strings(shell, curr_tkn, arg_list) == NO)
		{
			ft_lstclear(arg_list, free);
			return (free(*arg_list), NO);
		}
		// if (split_str_to_list(new_arg, arg_list) == NO)
		// {
		// 	ft_lstclear(arg_list, free);
		// 	return (free(*arg_list), NO);
		// }
		// int i = 0;
		// while (new_args[i])
		// 	ft_lstadd_back(arg_list, ft_lstnew(new_args[i++]));
		// free(new_args);
		curr_tkn = curr_tkn->next;
	}
	return (OK);
}

// int	collect_tokens_in_cmd_old(t_shell *shell, t_list **arg_list,
// 	t_token *start_tkn)
// {
// 	t_token	*curr_tkn;
// 	char	*new_arg;

// 	curr_tkn = start_tkn;
// 	while (curr_tkn && curr_tkn->type != TOKEN_END)
// 	{
// 		new_arg = collect_pieces_to_string(shell, curr_tkn);
// 		if (new_arg == NULL)
// 		{
// 			ft_lstclear(arg_list, free);
// 			return (free(*arg_list), NO);
// 		}
// 		// if (split_str_to_list(new_arg, arg_list) == NO)
// 		// {
// 		// 	ft_lstclear(arg_list, free);
// 		// 	return (free(*arg_list), NO);
// 		// }
// 		ft_lstadd_back(arg_list, ft_lstnew(new_arg));
// 		curr_tkn = curr_tkn->next;
// 	}
// 	return (OK);
// }

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
	}
	else
	{
		free_str_array(cmd->argv);
		cmd->argv = NULL;
	}
	ft_lstclear(&arg_list, empty_func);
	return (OK);
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
		// if (rebuild_argv_in_cmd(cmd, shell) == NO)
		// if (collect_argv_in_cmd(cmd, shell) == NO)
		if (collect_argv(shell, cmd) == NO)
		{
			shell->free(shell);
			exit(EXIT_FAILURE);
		}
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
			// if (collect_argv(cmd, shell) == NO)
			// {
			// 	shell->free(shell);
			// 	exit(EXIT_FAILURE);
			// }
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
	// Always execute external commands in a fork to avoid replacing the shell process ??
	return (execute_single_in_fork(cmd, shell, input_fd, output_fd));
}
