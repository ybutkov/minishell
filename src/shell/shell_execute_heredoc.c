/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 19:23:27 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/03 20:21:55 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_internal.h"
#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "shell.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

static int	is_delimiter(char *line, char *delimeter)
{
	char	*new_line_char;

	new_line_char = ft_strchr(line, '\n');
	if (new_line_char)
		*new_line_char = '\0';
	if (ft_strcmp(line, delimeter) == 0)
	{
		if (new_line_char)
			*new_line_char = '\n';
		return (1);
	}
	if (new_line_char)
		*new_line_char = '\n';
	return (0);
}

static void	collect_heredoc_input(char *target, int write_fd)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		// printf("%s", "heredoc>");
		line = get_next_line(STDIN_FILENO);
		// line = readline(">");
		if (!line)
			break ;
		if (is_delimiter(line, target))
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		free(line);
	}
}

static int	collect_heredoc_child(t_shell *shell, char *target,
	int heredoc_pipe[2])
{
	close(heredoc_pipe[0]);
	collect_heredoc_input(target, heredoc_pipe[1]);
	close(heredoc_pipe[1]);
	shell->free(shell);
	exit(EXIT_SUCCESS);
	return (0);
}

int	execute_redir_heredoc(t_shell *shell, t_redir *redirect, int out_fd)
{
	int				heredoc_pipe[2];
	pid_t			pid_writer;
	int				status;
	int				ret_code;

	ret_code = 1;
	if (pipe(heredoc_pipe) == -1)
		return (EXIT_FAILURE);
	pid_writer = fork();
	if (pid_writer == -1)
		return (close_fds(heredoc_pipe), ret_code);
	if (pid_writer == 0)
		return (collect_heredoc_child(shell, redirect->target, heredoc_pipe));
	else
	{
		close(heredoc_pipe[1]);
		waitpid(pid_writer, &status, 0);
		dup2_and_close(heredoc_pipe[0], out_fd);
		close(heredoc_pipe[0]);
		return (status);
	}
}
