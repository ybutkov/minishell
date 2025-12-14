/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 19:23:27 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/04 13:59:21 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_internal.h"
#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "shell.h"
#include "utils.h"
#include "shell_utils.h"
#include "constants.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	collect_heredoc_node(t_ast_node *node, t_shell *shell);

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

static int	open_file(char *filename, int flags, t_shell *shell)
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
	return (fd);
}

static void	collect_heredoc_input(char *target, int write_fd)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
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

int	execute_redir_heredoc(t_shell *shell, t_redir *redirect)
{
	int		fd;
	char	*file_name;

	file_name = get_tmp_file_name(get_file_n(1));
	if (file_name == NULL)
		return (EXIT_FAILURE);
	fd = open_file(file_name, O_WRONLY | O_CREAT | O_TRUNC, shell);
	collect_heredoc_input(redirect->target, fd);
	close(fd);
	redirect->target = file_name;
	return (EXIT_SUCCESS);
}

int collect_heredoc_subshell(t_ast_node *node, t_shell *shell)
{
	return (collect_heredoc_node(node->get_left(node), shell));
}

int	collect_heredoc_double(t_ast_node *node, t_shell *shell)
{
	int	status;

	if (node->get_left(node))
		status = collect_heredoc_node(node->get_left(node), shell);
	if (node->get_left(node))
		status = collect_heredoc_node(node->get_right(node), shell);
	return (status);
}

static int	apply_heredoc(t_cmd *cmd, t_shell *shell)
{
	t_list	*redir;
	t_redir	*redirect;
	int		status;

	status = 0;
	if (!cmd)
		return (EXIT_FAILURE);
	redir = cmd->redirs;
	while (redir)
	{
		redirect = (t_redir *)redir->content;
		if (redirect->type == REDIR_HEREDOC)
		{
			status = execute_redir_heredoc(shell, redirect);
		}
		redir = redir->next;
	}
	return (status);
}

int	collect_heredoc_cmd(t_cmd *cmd, t_shell *shell)
{
	return (apply_heredoc(cmd, shell));
}

int	collect_heredoc_node(t_ast_node *node, t_shell *shell)
{
	t_shell_node	*shell_node;

	shell_node = (t_shell_node *)node->get_content(node);
	// create array of wrapper funcs
	if (shell_node->type == NODE_SUBSHELL)
		return (collect_heredoc_subshell(node, shell));
	if (shell_node->type == NODE_PIPE)
		return (collect_heredoc_double(node, shell));
	if (shell_node->type == NODE_SEMICOLON)
		return (collect_heredoc_double(node, shell));
	if (shell_node->type == NODE_AND)
		return (collect_heredoc_double(node, shell));
	else if (shell_node->type == NODE_OR)
		return (collect_heredoc_double(node, shell));
	else if (shell_node->type == NODE_CMD)
		return (collect_heredoc_cmd(shell_node->data.cmd, shell));
	return (1);
}
