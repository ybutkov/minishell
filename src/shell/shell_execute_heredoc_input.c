/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_heredoc_input.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:20:25 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 04:36:14 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "parsing.h"
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

static void	error_heredoc_delimiter(char *target)
{
	char	*tmp;
	char	*str;

	tmp = ft_strjoin(HRDOC_DELIMITED_EOF, target);
	str = ft_strjoin(tmp, "')");
	free(tmp);
	output_error(MSG_WARNING_2_COLON, str);
	free(str);
}

static void	restore_signals_and_stdin(int the_stdin)
{
	dup2_and_close(the_stdin, STDIN_FILENO);
	set_signals_parent_interactive();
}

static char	*read_heredoc_line(void)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		line = readline(HEREDOC_PROMT);
	else
		line = get_next_line(STDIN_FILENO);
	return (line);
}

static int	input_heredoc(t_redir *redir, int write_fd, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (g_heredoc_interrupted)
		{
			free(line);
			return (EXIT_TERMINATED_BY_SIGINT);
		}
		if (!line && isatty(STDIN_FILENO))
			error_heredoc_delimiter(redir->target);
		if (!line)
			break ;
		if (!isatty(STDIN_FILENO) && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strcmp(line, redir->target) == 0)
			return (free(line), 0);
		if (redir->quotes == NO_QUOTES)
			expand_heredoc_line(&line, shell);
		write(write_fd, line, ft_strlen(line));
		write(write_fd, NEW_LINE, ft_strlen(NEW_LINE));
		free(line);
	}
	return (0);
}

int	collect_heredoc_input(t_redir *redir, int write_fd, t_shell *shell)
{
	int		the_stdin;

	the_stdin = dup(STDIN_FILENO);
	set_signals_heredoc();
	g_heredoc_interrupted = 0;
	rl_done = 0;
	if (input_heredoc(redir, write_fd, shell) == EXIT_TERMINATED_BY_SIGINT)
	{
		restore_signals_and_stdin(the_stdin);
		shell->ctx->last_exit_status = EXIT_TERMINATED_BY_SIGINT;
		return (EXIT_TERMINATED_BY_SIGINT);
	}
	restore_signals_and_stdin(the_stdin);
	return (0);
}
