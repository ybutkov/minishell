/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:57:40 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/13 23:53:10 by ashadrin         ###   ########.fr       */
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
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

static void	open_file_and_dup2(char *filename, int flags, int dup_fd,
		t_shell *shell)
{
	int	fd;
	int	mode;

	if (flags & O_CREAT)
		mode = 0644;
	else
		mode = 0;
	fd = open(filename, flags, mode);
	if (fd == -1)
		output_error_and_exit(filename, NULL, shell, EXIT_FAILURE);
	dup2(fd, dup_fd);
	close(fd);
}

static void	apply_redirect(t_cmd *cmd, t_shell *shell)
{
	t_list	*redir;
	t_redir	*redirect;

	if (!cmd)
		return ;
	redir = cmd->redirs;
	while (redir)
	{
		redirect = (t_redir *)redir->content;
		if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
			open_file_and_dup2(redirect->target, O_RDONLY, STDIN_FILENO, shell);
		else if (redirect->type == REDIR_OUT)
			open_file_and_dup2(redirect->target, O_WRONLY | O_CREAT | O_TRUNC,
				STDOUT_FILENO, shell);
		else if (redirect->type == REDIR_APPEND)
			open_file_and_dup2(redirect->target, O_WRONLY | O_CREAT | O_APPEND,
				STDOUT_FILENO, shell);
		else if (redirect->type == REDIR_HEREDOC)
		{
			// execute_redir_heredoc(shell, redirect, STDIN_FILENO);
		}
		redir = redir->next;
	}
}

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

// void	dup2_and_close(int oldfd, int newfd)
// {
// 	dup2(oldfd, newfd);
// 	close(oldfd);
// }

// static int	execute_cmd_child(t_cmd *cmd, t_shell *shell, int input_fd,
// 		int output_fd)
// {
// 	if (input_fd != STDIN_FILENO)
// 	{
// 		dup2(input_fd, STDIN_FILENO);
// 		close(input_fd);
// 	}
// 	if (output_fd != STDOUT_FILENO)
// 	{
// 		dup2(output_fd, STDOUT_FILENO);
// 		close(output_fd);
// 	}
// 	apply_redirect(cmd, shell);
// 	if (is_builtin(cmd->argv[0]))
// 		return (builtin(cmd, shell, input_fd, output_fd));
// 	if (!cmd->path || access(cmd->path, X_OK) != 0)
// 		output_error_and_exit(cmd->argv[0], CMD_NOT_FOUND_MSG, shell,
// 			EXIT_CMD_NOT_FOUND);
// 	execve(cmd->path, cmd->argv, shell->ctx->envp);
// 	shell->free(shell);
// 	exit(EXIT_FAILURE);
// }

// int	execute_cmd(t_cmd *cmd, t_shell *shell, int input_fd, int output_fd)
// {
// 	return (execute_cmd_child(cmd, shell, input_fd, output_fd));
// }

// void	dup2_and_close(int oldfd, int newfd)
// {
// 	dup2(oldfd, newfd);
// 	close(oldfd);
// }

int	execute_single_in_fork(t_cmd *cmd, t_shell *shell, int input_fd,
		int output_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
			dup2_and_close(input_fd, STDIN_FILENO);
		if (output_fd != STDOUT_FILENO)
			dup2_and_close(output_fd, STDOUT_FILENO);
		else
			dup2(STDOUT_FILENO, STDOUT_FILENO);
		apply_redirect(cmd, shell);
		if (!cmd->path || access(cmd->path, X_OK) != 0)
			output_error_and_exit(cmd->argv[0], CMD_NOT_FOUND_MSG, shell,
				EXIT_CMD_NOT_FOUND);
		execve(cmd->path, cmd->argv, shell->ctx->envp);
		shell->free(shell);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}

// static void	apply_heredoc(t_cmd *cmd, t_shell *shell)
// {
// 	t_list	*redir;
// 	t_redir	*redirect;

// 	if (!cmd)
// 		return ;
// 	redir = cmd->redirs;
// 	while (redir)
// 	{wrapper
// 		redirect = (t_redir *)redir->content;
// 		if (redirect->type == REDIR_HEREDOC)
// 		{
// 			execute_redir_heredoc(shell, redirect, STDIN_FILENO);
// 		}
// 		redir = redir->next;
// 	}
// }

int	execute_cmd(t_cmd *cmd, t_shell *shell, int input_fd, int output_fd)
{
	int	bi_func;
////////////////////////////////////////////////////
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		write(STDERR_FILENO, ": command not found\n", 20);
		shell->ctx->last_exit_status = 127;
		return (127);
	}
//////////////////////////////////////////////////////////
	bi_func = builtin_func(cmd->argv[0]);
	if (bi_func != -1)
		return (builtin(bi_func, cmd, shell, input_fd, output_fd));
	// check for single command. execute in sep fork
	if (input_fd == STDIN_FILENO && output_fd == STDOUT_FILENO)
		return (execute_single_in_fork(cmd, shell, input_fd, output_fd));
	// apply_heredoc(cmd, shell);
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	apply_redirect(cmd, shell);
	if (!cmd->path || access(cmd->path, X_OK) != 0)
		output_error_and_exit(cmd->argv[0], CMD_NOT_FOUND_MSG, shell,
			EXIT_CMD_NOT_FOUND);
	execve(cmd->path, cmd->argv, shell->ctx->envp);
	shell->free(shell);
	exit(EXIT_FAILURE);
}
