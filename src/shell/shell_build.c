/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:53:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/15 02:50:36 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "libft.h"
#include "parsing.h"
#include "shell_internal.h"

t_ast_node	*build_ast(t_shell *shell, t_ast_node **node, t_token *start_tkn,
				t_token *end_tkn);

static t_token	*find_paren(t_token *start_tkn)
{
	t_token	*curr;
	int		depth;

	if (!start_tkn || start_tkn->type != TOKEN_LEFT_PAREN)
		return (NULL);
	curr = start_tkn->next;
	depth = 1;
	while (curr && depth > 0)
	{
		if (curr->type == TOKEN_LEFT_PAREN)
			depth++;
		else if (curr->type == TOKEN_RIGHT_PAREN)
			depth--;
		if (depth > 0)
			curr = curr->next;
	}
	return (curr);
}

static void	add_subshell_redir(t_shell_node *node, t_redir *redirect)
{
	t_list	*redir;

	redir = ft_lstnew(redirect);
	if (node->redirs)
		ft_lstadd_back(&node->redirs, redir);
	else
		node->redirs = redir;
}

static void	collect_subshell_redirs(t_shell_node *node, t_token *start,
		t_token *end)
{
	t_token			*curr;
	t_redir			*redirect;
	t_redir_type	type;

	curr = start;
	while (curr && curr != end && curr->type != TOKEN_END)
	{
		type = -1;
		if (curr->type == TOKEN_REDIR_OUT && curr->next
			&& curr->next->type == TOKEN_WORD)
			type = REDIR_OUT;
		else if (curr->type == TOKEN_REDIR_APPEND && curr->next
			&& curr->next->type == TOKEN_WORD)
			type = REDIR_APPEND;
		else if (curr->type == TOKEN_REDIR_IN && curr->next
			&& curr->next->type == TOKEN_WORD)
			type = REDIR_IN;
		else if (curr->type == TOKEN_HEREDOC && curr->next
			&& curr->next->type == TOKEN_WORD)
			type = REDIR_HEREDOC;
		if (type != (t_redir_type)-1)
		{
			redirect = create_redir(type, curr->next->value);
			add_subshell_redir(node, redirect);
			curr = curr->next;
		}
		curr = curr->next;
	}
}

static int	is_subshell_start(t_token *start_tkn)
{
	t_token	*closed_paren;

	if (!start_tkn || start_tkn->type != TOKEN_LEFT_PAREN)
		return (0);
	closed_paren = find_paren(start_tkn);
	return (closed_paren != NULL);
}

void	put_shell_node_to_ast(t_ast_node **curr_node, t_shell_node *node, int i,
		int argc)
{
	t_ast_node		*ast_node;
	t_shell_node	*pipe_node;
	t_ast_node		*pipe_ast;

	ast_node = create_ast_node(node);
	if (i == 1)
		(*curr_node)->set_left((*curr_node), ast_node);
	else if (i == argc - 2)
		(*curr_node)->set_right((*curr_node), ast_node);
	else
	{
		pipe_node = create_shell_node(NODE_PIPE, NULL);
		pipe_ast = create_ast_node(pipe_node);
		(*curr_node)->set_right((*curr_node), pipe_ast);
		pipe_ast->set_left(pipe_ast, ast_node);
		*curr_node = pipe_ast;
	}
}


t_token	*get_next_token_for_lvl(t_token *start_token, t_token *end_token,
		int lvl)
{
	t_token			*curr_token;
	t_token			*found_token;
	t_token_type	lvl_start;
	t_token_type	lvl_end;
	char			is_subshell_opened;

	if (lvl == 1)
	{
		lvl_start = TKN_LVL_1_FROM;
		lvl_end = TKN_LVL_1_TO;
	}
	else if (lvl == 2)
	{
		lvl_start = TKN_LVL_2_FROM;
		lvl_end = TKN_LVL_2_TO;
	}
	else if (lvl == 3)
	{
		lvl_start = TKN_LVL_3_FROM;
		lvl_end = TKN_LVL_3_TO;
	}
	else
		return (NULL);
	curr_token = start_token;
	found_token = NULL;
	is_subshell_opened = 0;
	while (curr_token != end_token)
	{
		if (curr_token && curr_token->type == TOKEN_LEFT_PAREN)
		{
			is_subshell_opened += 1;
			curr_token = curr_token->next;
			continue ;
		}
		else if (is_subshell_opened && curr_token
			&& curr_token->type == TOKEN_RIGHT_PAREN)
		{
			is_subshell_opened -= 1;
			curr_token = curr_token->next;
			continue ;
		}
		if (!is_subshell_opened && curr_token && curr_token->type >= lvl_start
			&& curr_token->type <= lvl_end)
		{
			if (lvl == 1)
				found_token = curr_token;
			else
				return (curr_token);
		}
		curr_token = curr_token->next;
	}
	if (curr_token && curr_token == end_token && curr_token->type >= lvl_start
		&& curr_token->type <= lvl_end && !is_subshell_opened)
	{
		if (lvl == 1)
			found_token = curr_token;
		else
			return (curr_token);
	}
	return (found_token);
}

char	**parse_tokens_to_argv(t_token *start_tkn, t_token *end_tkn)
{
	char	**argv;
	t_token	*curr_tkn;
	int		i;

	curr_tkn = start_tkn;
	i = 1;
	while (curr_tkn != end_tkn && curr_tkn->type != TOKEN_END)
	{
		i++;
		curr_tkn = curr_tkn->next;
	}
	argv = malloc(sizeof(char *) * (i + 1));
	if (!argv)
		return (NULL);
	curr_tkn = start_tkn;
	i = 0;
	while (curr_tkn != end_tkn && curr_tkn->type != TOKEN_END)
	{
		argv[i] = ft_strdup(curr_tkn->value);
		curr_tkn = curr_tkn->next;
		i++;
	}
	if (curr_tkn->type != TOKEN_END)
	{
		argv[i] = ft_strdup(curr_tkn->value);
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

static void	add_redirect(t_cmd *cmd, t_redir *redirect)
{
	t_list	*redir;

	redir = ft_lstnew(redirect);
	if (cmd->redirs)
		ft_lstadd_back(&cmd->redirs, redir);
	else
		cmd->redirs = redir;
}

int	check_redir_token_type(t_token *token, t_token_type redir_token_type)
{
	if (token->type == redir_token_type && token->next
		&& token->next->type == TOKEN_WORD)
		return (1);
	return (0);
}

int	collect_redirs(t_cmd *cmd, t_token **start_tkn, t_token **end_tkn)
{
	t_redir			*redirect;
	t_redir_type	type;
	t_token			*tmp_tkn;
	int				running;
	t_token			*curr_tkn;

	running = 1;
	curr_tkn = *start_tkn;
	while (running && curr_tkn && curr_tkn != *end_tkn
		&& curr_tkn->type != TOKEN_END)
	{
		if (check_redir_token_type(curr_tkn, TOKEN_REDIR_OUT))
			type = REDIR_OUT;
		else if (check_redir_token_type(curr_tkn, TOKEN_REDIR_APPEND))
			type = REDIR_APPEND;
		else if (check_redir_token_type(curr_tkn, TOKEN_REDIR_IN))
			type = REDIR_IN;
		else if (check_redir_token_type(curr_tkn, TOKEN_HEREDOC))
			type = REDIR_HEREDOC;
		else
		{
			// Error  ???
			curr_tkn = curr_tkn->next;
			continue ;
		}
		if (curr_tkn->next == NULL || curr_tkn->next->type != TOKEN_WORD)
			return (ERROR);
		redirect = create_redir(type, curr_tkn->next->value);
		add_redirect(cmd, redirect);
		tmp_tkn = curr_tkn->next->next;
		if (curr_tkn->prev)
			curr_tkn->prev->next = tmp_tkn;
		if (tmp_tkn)
			tmp_tkn->prev = curr_tkn->prev;
		if (curr_tkn->next == (*end_tkn))
		{
			running = 0;
			(*end_tkn) = curr_tkn->prev;
		}
		if (curr_tkn == *start_tkn)
			*start_tkn = tmp_tkn;
		curr_tkn->next->free(curr_tkn->next);
		curr_tkn->free(curr_tkn);
		curr_tkn = tmp_tkn;
	}
	return (OK);
}

/*
t_cmd	*parse_tokens_to_cmd(t_shell *shell, t_token *start_tkn,
		t_token *end_tkn)
{
	t_cmd	*cmd;
	char	**argv;
	char	*path;
	t_token	*curr_tkn;
	int		i;

	cmd = create_cmd(NULL, NULL);
	if (collect_redirs(cmd, &start_tkn, &end_tkn) == ERROR)
		return (NULL); // handle error
	curr_tkn = start_tkn;
	i = 1;
	while (curr_tkn != end_tkn && curr_tkn->type != TOKEN_END)
	{
		i++;
		curr_tkn = curr_tkn->next;
	}
	argv = malloc(sizeof(char *) * (i + 1));
	if (!argv)
		return (NULL);
	curr_tkn = start_tkn;
	i = 0;
	while (curr_tkn != end_tkn && curr_tkn->type != TOKEN_END)
	{
		argv[i] = ft_strdup(curr_tkn->value);
		curr_tkn = curr_tkn->next;
		i++;
	}
	if (curr_tkn->type != TOKEN_END)
	{
		argv[i] = ft_strdup(curr_tkn->value);
		i++;
	}
	argv[i] = NULL;
	/////////////////////////////////////
	if (!argv[0])
	{
		cmd->argv = argv;
		cmd->path = NULL;
		return (cmd);
	}
	///////////////////////////////////////
	path = get_cmd_path(argv[0], shell->ctx->envp);
	cmd->argv = argv;
	cmd->path = path;
	return (cmd);
}
*/

//----------------------------------------------------------------------

t_cmd	*parse_tokens_to_cmd(t_shell *shell, t_token *start_tkn,
		t_token *end_tkn)
{
	t_cmd	*cmd;
	char	**argv;
	char	*path;
	t_token	*curr_tkn;
	int		total_args;
	char	**expanded_args;
	int		i;
	t_list	*node;
	t_list 	*arg_list;

	arg_list = NULL;
	total_args = 0;
	cmd = create_cmd(NULL, NULL);
	if (collect_redirs(cmd, &start_tkn, &end_tkn) == ERROR)
		return (NULL);
	curr_tkn = start_tkn;
	while (curr_tkn && curr_tkn->type != TOKEN_END)
	{
		if (curr_tkn->pieces)
		{
			expanded_args = expand_and_split_token(curr_tkn, shell->ctx->env,
					shell->ctx->last_exit_status);
			if (!expanded_args)
			{
				ft_lstclear(&arg_list, free);
				return (NULL);
			}
			i = 0;
			char *new_arg = ft_strdup("");
			while (expanded_args[i])
			{
				ft_strappend(&new_arg, expanded_args[i]);
				free(expanded_args[i]);
				i++;
			}
			free(expanded_args);
			total_args++;
			ft_lstadd_back(&arg_list, ft_lstnew(new_arg));
		}
		else
		{
			ft_lstadd_back(&arg_list, ft_lstnew(ft_strdup(curr_tkn->value)));
			total_args++;
		}
		if (curr_tkn == end_tkn)
			break ;
		curr_tkn = curr_tkn->next;
	}
	argv = malloc(sizeof(char *) * (total_args + 1));
	if (!argv)
	{
		ft_lstclear(&arg_list, free);
		return (NULL);
	}
	node = arg_list;
	i = 0;
	while (node)
	{
		argv[i++] = (char *)node->content;
		node = node->next;
	}
	argv[i] = NULL;
	ft_lstclear(&arg_list, NULL);
	if (i > 0 && argv[0][0] != '\0')
	{
		path = get_cmd_path(argv[0], shell->ctx->envp);
		cmd->argv = argv;
		cmd->path = path;
	}
	return (cmd);
}

//--------------------------------------------------------------

t_cmd	*create_cmd_from_tokens(t_shell *shell, t_token *start_tkn,
		t_token *end_tkn)
{
	t_cmd	*cmd;

	cmd = parse_tokens_to_cmd(shell, start_tkn, end_tkn);
	return (cmd);
}

int	create_node_for_subshell(t_shell *shell, t_ast_node **node,
		t_token *start_tkn, t_token *end_tkn)
{
	t_shell_node	*shell_node;
	t_ast_node		*left_node;
	t_token			*matching_paren;

	shell_node = create_shell_node(NODE_SUBSHELL, NULL);
	(*node)->set_content(*node, shell_node);
	left_node = create_ast_node(NULL);
	(*node)->set_left(*node, left_node);
	matching_paren = find_paren(start_tkn);
	if (matching_paren && matching_paren->next && matching_paren != end_tkn)
		collect_subshell_redirs(shell_node, matching_paren->next, end_tkn->next);
	build_ast(shell, &left_node, start_tkn->next, matching_paren->prev);
	return (1);
}

int	create_ast_node_for_lvl(t_shell *shell, t_ast_node **node,
		t_token *start_tkn, t_token *end_tkn, int lvl)
{
	t_token			*curr_tkn;
	t_ast_node		*left_node;
	t_ast_node		*right_node;
	t_shell_node	*shell_node;

	curr_tkn = get_next_token_for_lvl(start_tkn, end_tkn, lvl);
	if (curr_tkn)
	{
		if (lvl < 3)
		{
			shell_node = create_shell_node(get_node_type_by_token(curr_tkn->type),
					NULL);
			(*node)->set_content(*node, shell_node);
			left_node = create_ast_node(NULL);
			right_node = create_ast_node(NULL);
			(*node)->set_left(*node, left_node);
			(*node)->set_right(*node, right_node);
			build_ast(shell, &left_node, start_tkn, curr_tkn->prev);
			build_ast(shell, &right_node, curr_tkn->next, end_tkn);
			return (1);
		}
		if (is_subshell_start(start_tkn))
			return (create_node_for_subshell(shell, node, start_tkn, end_tkn));
		shell_node = create_shell_node(NODE_CMD, NULL);
		(*node)->set_content(*node, shell_node);
		shell_node->data.cmd = create_cmd_from_tokens(shell, start_tkn,
				end_tkn);
		return (1);
	}
	if (lvl >= 3 && is_subshell_start(start_tkn))
		return (create_node_for_subshell(shell, node, start_tkn, end_tkn));
	return (0);
}

t_ast_node	*build_ast(t_shell *shell, t_ast_node **node, t_token *start_tkn,
		t_token *end_tkn)
{
	if (*node == NULL)
		*node = create_ast_node(NULL);
	if (create_ast_node_for_lvl(shell, node, start_tkn, end_tkn, 1) == 1)
		return (*node);
	if (create_ast_node_for_lvl(shell, node, start_tkn, end_tkn, 2) == 1)
		return (*node);
	if (create_ast_node_for_lvl(shell, node, start_tkn, end_tkn, 3) == 1)
		return (*node);
	return (*node);
}

void	build_shell(t_shell *shell, t_token *token)
{
	t_ast_node	*root_node;
	t_token		*end_token;

	root_node = NULL;
	end_token = token;
	while (end_token->next)
		end_token = end_token->next;
	end_token = end_token->prev;
    if (!validate_tokens(token))
		return ;
	root_node = build_ast(shell, &root_node, token, end_token);
	shell->ast->set_root(shell->ast, root_node);
}
