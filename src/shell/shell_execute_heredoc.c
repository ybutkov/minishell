/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 19:23:27 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/22 20:43:05 by ybutkov          ###   ########.fr       */
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
#include "signals.h"
#include "parsing.h"
#include <errno.h>

int	collect_heredoc_node(t_ast_node *node, t_shell *shell);

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

void error_heredoc_delimiter(char *target)
{
	char	*tmp;
	char	*str;

	tmp = "here-document delimited by end-of-file (wanted '";
	tmp = ft_strjoin(tmp, target);
	str = ft_strjoin(tmp, "')");
	free(tmp);
	output_error("warning:", str);
	free(str);
}

char	*get_var_name(char *line, int *i)
{
	int		start;

	start = *i;
	while (!will_end_env_var(line[*i]))
		(*i)++;
	return (ft_substr(line, start, (size_t)(*i - start)));
}

int	expand_heredoc_line(char **line, t_shell *shell)
{
	char	*str;
	char	*value;
	char	*res;
	char	*append_str;
	int		i;
	int		start;

	i = 0;
	start = 0;
	res = NULL;
	value = NULL;
	while ((*line)[i])
	{
		if ((*line)[i] == '$'
			&& (ft_isalpha((*line)[i+1]) || (*line)[i+1] == '?'))
		{
			append_str = ft_substr((*line), start, i - start);
			i += 1;
			if ((*line)[i] == '?')
			{
				i += 1;
				start = i;
				ft_strappend(&res, append_str);
				free(append_str);
				append_str = ft_itoa(shell->ctx->last_exit_status);
				ft_strappend(&res, append_str);
				free(append_str);
				continue ;
			}
			str = get_var_name((*line), &i);
			if (str)
				value = shell->ctx->env->get_value(shell->ctx->env, str);
			free(str);
			if (value)
			{
				start = i;
				ft_strappend(&res, append_str);
				free(append_str);
				ft_strappend(&res, value);
				// add copy to get_value
				// free(value);
				continue ;
			}
		}
		i += 1;
	}
	if (start < i)
	{
		append_str = ft_substr((*line), start, i - start);
		ft_strappend(&res, append_str);
	}
	free(*line);
	*line = res;
	return (OK);
}

static int	collect_heredoc_input(t_redir *redir, int write_fd, t_shell *shell)
{
	char    *line = NULL;
	int     the_stdin;

	the_stdin = dup(STDIN_FILENO);
	set_signals_heredoc();
	g_heredoc_interrupted = 0;
	rl_done = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("heredoc> ");
		else
			line = get_next_line(STDIN_FILENO);
		if (g_heredoc_interrupted)
		{
			if (line)
				free(line);
			dup2(the_stdin, STDIN_FILENO);
			close(the_stdin);
			set_signals_parent_interactive();
			shell->ctx->last_exit_status = EXIT_TERMINATED_BY_SIGINT;
			return (EXIT_TERMINATED_BY_SIGINT);
		}
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				error_heredoc_delimiter(redir->target);
			break ;
		}
		if (!isatty(STDIN_FILENO))
		{
            if (line[ft_strlen(line) - 1] == '\n')
                line[ft_strlen(line) - 1] = '\0';
		}
		if (ft_strcmp(line, redir->target) == 0)
		{
			free(line);
			break ;
		}
		if (redir->quotes == NO_QUOTES)
			expand_heredoc_line(&line, shell);
		write(write_fd, line, ft_strlen(line));
		write(write_fd, NEW_LINE, ft_strlen(NEW_LINE));
		free(line);
	}
	dup2(the_stdin, STDIN_FILENO);
	close(the_stdin);
	set_signals_parent_interactive();
	return (0);
}

int	execute_redir_heredoc(t_shell *shell, t_redir *redirect)
{
	int		fd;
	int		status;
	char	*file_name;

	file_name = get_tmp_file_name(get_file_n(1));
	if (file_name == NULL)
		return (EXIT_FAILURE);
	fd = open_file(file_name, O_WRONLY | O_CREAT | O_TRUNC, shell);
	status = collect_heredoc_input(redirect, fd, shell);
	close(fd);
	if (status == EXIT_TERMINATED_BY_SIGINT)
	{
		unlink(file_name);
		free(file_name);
		return (EXIT_TERMINATED_BY_SIGINT);
	}
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
			if (status == EXIT_TERMINATED_BY_SIGINT)
				return (status);
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
