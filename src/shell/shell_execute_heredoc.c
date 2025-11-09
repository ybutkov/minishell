/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 19:23:27 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/09 19:23:29 by ybutkov          ###   ########.fr       */
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
		line = get_next_line(STDIN_FILENO);
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

static int	collect_heredoc_input_child(t_shell_node	*node, t_shell *shell,
	int heredoc_pipe[2])
{
	close(heredoc_pipe[0]);
	collect_heredoc_input(node->data.redir->target, heredoc_pipe[1]);
	close(heredoc_pipe[1]);
	shell->free(shell);
	exit(EXIT_SUCCESS);
	return (0);
}

int	execute_redir_heredoc(t_ast_node *ast_node, t_shell *shell, int in_fd,
		int old_fd_out)
{
	t_shell_node	*node;
	int				heredoc_pipe[2];
	pid_t			pid_writer;
	int				status;
	int				ret_code;

	(void)in_fd;
	ret_code = 1;
	node = (t_shell_node *)ast_node->get_content(ast_node);
	if (pipe(heredoc_pipe) == -1)
		return (EXIT_FAILURE);
	pid_writer = fork();
	if (pid_writer == -1)
		return (close_fds(heredoc_pipe), ret_code);
	if (pid_writer == 0)
		return (collect_heredoc_input_child(node, shell, heredoc_pipe));
	else
	{
		close(heredoc_pipe[1]);
		waitpid(pid_writer, &status, 0);
		ret_code = execute_shell_node(ast_node->get_left(ast_node), shell,
				heredoc_pipe[0], old_fd_out);
		close(heredoc_pipe[0]);
		return (ret_code);
	}
}
