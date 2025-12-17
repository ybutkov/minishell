/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:53:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/17 03:16:33 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "libft.h"
#include "parsing.h"
#include "shell_internal.h"
#include "utils.h"

t_ast_node		*build_ast(t_shell *shell, t_ast_node **node,
					t_token *start_tkn, t_token *end_tkn);

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

static int	is_redirect_type(t_token *curr, t_token_type type)
{
	if (curr->type == type && curr->next && curr->next->type == TOKEN_WORD)
		return (OK);
	else
		return (NO);
}

t_redir_type	get_only_redir_types(t_token *token)
{
	if (is_redirect_type(token, TOKEN_REDIR_OUT))
		return (REDIR_OUT);
	else if (is_redirect_type(token, TOKEN_REDIR_APPEND))
		return (REDIR_APPEND);
	else if (is_redirect_type(token, TOKEN_REDIR_IN))
		return (REDIR_IN);
	else if (is_redirect_type(token, TOKEN_HEREDOC))
		return (REDIR_HEREDOC);
	else
		return ((t_redir_type)(-1));
}

char	*collect_pieces_to_string(t_shell *shell, t_token *curr_tkn)
{
	char	**expanded_args;
	char	*new_arg;
	int		i;

	i = 0;
	if (curr_tkn->pieces)
	{
		expanded_args = expand_and_split_token(curr_tkn, shell->ctx->env,
				shell->ctx->last_exit_status);
		new_arg = ft_strdup("");
		if (!expanded_args || new_arg == NULL)
			return (free(expanded_args), free(new_arg), NULL);
		while (expanded_args[i])
		{
			if (ft_strappend(&new_arg, expanded_args[i++]) == 0)
			{
				free_str_array(expanded_args);
				return (free(new_arg), NULL);
			}
		}
		free_str_array(expanded_args);
	}
	else
		new_arg = ft_strdup(curr_tkn->value);
	return (new_arg);
}

t_redir	*create_redirect(t_shell *shell, t_redir_type type, t_token *curr_tkn)
{
	char	*target;

	target = collect_pieces_to_string(shell, curr_tkn);
	if (target == NULL)
		return (NULL);
	return (create_redir(type, target));
}

static void	collect_subshell_redirs(t_shell *shell, t_shell_node *node,
		t_token *start, t_token *end)
{
	t_token			*curr;
	t_redir			*redirect;
	t_redir_type	type;

	curr = start;
	while (curr && curr != end && curr->type != TOKEN_END)
	{
		type = get_only_redir_types(curr);
		if (type != (t_redir_type)(-1))
		{
			redirect = create_redirect(shell, type, curr->next);
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

// char	**parse_tokens_to_argv(t_token *start_tkn, t_token *end_tkn)
// {
// 	char	**argv;
// 	t_token	*curr_tkn;
// 	int		i;

// 	curr_tkn = start_tkn;
// 	i = 1;
// 	while (curr_tkn != end_tkn && curr_tkn->type != TOKEN_END)
// 	{
// 		i++;
// 		curr_tkn = curr_tkn->next;
// 	}
// 	argv = malloc(sizeof(char *) * (i + 1));
// 	if (!argv)
// 		return (NULL);
// 	curr_tkn = start_tkn;
// 	i = 0;
// 	while (curr_tkn != end_tkn && curr_tkn->type != TOKEN_END)
// 	{
// 		argv[i] = ft_strdup(curr_tkn->value);
// 		curr_tkn = curr_tkn->next;
// 		i++;
// 	}
// 	if (curr_tkn->type != TOKEN_END)
// 	{
// 		argv[i] = ft_strdup(curr_tkn->value);
// 		i++;
// 	}
// 	argv[i] = NULL;
// 	return (argv);
// }

static void	add_redirect(t_cmd *cmd, t_redir *redirect)
{
	t_list	*redir;

	redir = ft_lstnew(redirect);
	if (cmd->redirs)
		ft_lstadd_back(&cmd->redirs, redir);
	else
		cmd->redirs = redir;
}

static void	cut_redir_tokens(t_token **curr_tkn, t_token **start_tkn,
		t_token **end_tkn, int *running)
{
	t_token	*tmp_tkn;

	tmp_tkn = (*curr_tkn)->next->next;
	if ((*curr_tkn)->prev)
		(*curr_tkn)->prev->next = tmp_tkn;
	if (tmp_tkn)
		tmp_tkn->prev = (*curr_tkn)->prev;
	if ((*curr_tkn)->next == (*end_tkn))
	{
		*running = 0;
		(*end_tkn) = (*curr_tkn)->prev;
	}
	if (*curr_tkn == *start_tkn)
		*start_tkn = tmp_tkn;
	(*curr_tkn)->next->free((*curr_tkn)->next);
	(*curr_tkn)->free(*curr_tkn);
	(*curr_tkn) = tmp_tkn;
}

int	collect_redirs(t_shell *shell, t_cmd *cmd, t_token **start_tkn,
		t_token **end_tkn)
{
	t_redir			*redirect;
	t_redir_type	type;
	int				running;
	t_token			*curr_tkn;

	running = 1;
	curr_tkn = *start_tkn;
	while (running && curr_tkn && curr_tkn != *end_tkn
		&& curr_tkn->type != TOKEN_END)
	{
		type = get_only_redir_types(curr_tkn);
		if (type == (t_redir_type)(-1))
		{
			curr_tkn = curr_tkn->next;
			continue ;
		}
		if (curr_tkn->next == NULL || curr_tkn->next->type != TOKEN_WORD)
			return (ERROR);
		redirect = create_redirect(shell, type, curr_tkn->next);
		add_redirect(cmd, redirect);
		cut_redir_tokens(&curr_tkn, start_tkn, end_tkn, &running);
	}
	return (OK);
}

// int	split_arg_list(t_list *arg_list, int *total_args)
// {
// 	char	**argv;
// 	t_list	*node;
// 	t_list	*prev_node;
// 	t_list	*tmp_node;
// 	int		i;
// 	int		j;
// 	char	*arg;

// 	(void)total_args;
// 	node = arg_list;
// 	i = 0;
// 	prev_node = NULL;
// 	while (node)
// 	{
// 		arg = (char *)node->content;
// 		if (ft_strchr(arg, ' ') != NULL)
// 		{
// 			argv = ft_split(arg, ' ');
// 			j = 0;
// 			while (argv[j])
// 			{
// 				tmp_node = ft_lstnew(argv[j++]);
// 				prev_node->next = tmp_node;
// 				prev_node = tmp_node;
// 			}
// 			prev_node->next = node->next;
// 			ft_lstdelone(node, free);
// 			free(argv);
// 			node = prev_node->next;
// 			continue ;
// 		}
// 		prev_node = node;
// 		node = node->next;
// 	}
// 	return (OK);
// }

// int	split_arg_list2(t_list *arg_list, int *total_args)
// {
// 	char	**argv;
// 	t_list	*node;
// 	t_list	*tmp_node;
// 	t_list	*last_new;
// 	int		count;
// 	int		k;
// 	char	*arg;
// 	t_list	*remainder;

// 	node = arg_list;
// 	while (node)
// 	{
// 		arg = (char *)node->content;
// 		if (arg && ft_strchr(arg, ' ') != NULL)
// 		{
// 			argv = ft_split(arg, ' ');
// 			if (!argv)
// 				return (0);
// 			count = 0;
// 			while (argv[count])
// 				count++;
// 			if (count == 0)
// 			{
// 				free(argv);
// 				node = node->next;
// 				continue ;
// 			}
// 			free(node->content);
// 			node->content = argv[0];
// 			last_new = node;
// 			k = 1;
// 			while (k < count)
// 			{
// 				tmp_node = ft_lstnew(argv[k]);
// 				last_new->next = tmp_node;
// 				last_new = tmp_node;
// 				k++;
// 			}
// 			remainder = node->next;
// 			if (remainder == node)
// 				remainder = NULL;
// 			last_new->next = remainder;
// 			if (total_args)
// 				*total_args += (count - 1);
// 			free(argv);
// 			node = last_new->next;
// 			continue ;
// 		}
// 		node = node->next;
// 	}
// 	return (OK);
// }

char	**collect_tokens_to_argv(t_shell *shell, t_token *start_tkn,
	t_token *end_tkn)
{
	t_token	*curr_tkn;
	char	*new_arg;
	t_list	*arg_list;
	char	**argv;

	arg_list = NULL;
	curr_tkn = start_tkn;
	while (curr_tkn && curr_tkn->type != TOKEN_END)
	{
		new_arg = collect_pieces_to_string(shell, curr_tkn);
		if (new_arg == NULL)
		{
			ft_lstclear(&arg_list, free);
			return (free(arg_list), HANDLE_ERROR_NULL);
		}
		ft_lstadd_back(&arg_list, ft_lstnew(new_arg));
		if (curr_tkn == end_tkn)
			break ;
		curr_tkn = curr_tkn->next;
	}
	// if (split_arg_list2(arg_list, &total_args) != OK)
	// {
	// 	// Handle error
	// }
	argv = list_to_array(arg_list);
	ft_lstclear(&arg_list, empty_func);
	return (argv);
}

t_cmd	*create_cmd_from_tokens(t_shell *shell, t_token *start_tkn,
		t_token *end_tkn)
{
	t_cmd	*cmd;
	char	**argv;
	char	*path;

	cmd = create_cmd(NULL, NULL);
	if (collect_redirs(shell, cmd, &start_tkn, &end_tkn) == ERROR)
		return (cmd->free_cmd(cmd), HANDLE_ERROR_NULL);
	argv = collect_tokens_to_argv(shell, start_tkn, end_tkn);
	if (!argv)
		return (cmd->free_cmd(cmd), HANDLE_ERROR_NULL);
	if (argv[0] && argv[0][0] != '\0')
	{
		path = get_cmd_path(argv[0], shell->ctx->envp);
		cmd->argv = argv;
		cmd->path = path;
	}
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
		collect_subshell_redirs(shell, shell_node, matching_paren->next,
			end_tkn->next);
	build_ast(shell, &left_node, start_tkn->next, matching_paren->prev);
	return (1);
}

void	create_leaves(t_shell *shell, t_ast_node **node, t_token *curr_tkn,
		t_token **start_end_tokens)
{
	t_shell_node	*shell_node;
	t_ast_node		*left_node;
	t_ast_node		*right_node;

	shell_node = create_shell_node(get_node_type(curr_tkn->type), NULL);
	(*node)->set_content(*node, shell_node);
	left_node = create_ast_node(NULL);
	right_node = create_ast_node(NULL);
	(*node)->set_left(*node, left_node);
	(*node)->set_right(*node, right_node);
	build_ast(shell, &left_node, start_end_tokens[0], curr_tkn->prev);
	build_ast(shell, &right_node, curr_tkn->next, start_end_tokens[1]);
}

int	create_ast_node_for_lvl(t_shell *shell, t_ast_node **node,
		t_token **start_end_tokens, int lvl)
{
	t_token			*curr_tkn;
	t_shell_node	*shell_node;

	curr_tkn = get_next_token_for_lvl(start_end_tokens[0], start_end_tokens[1],
			lvl);
	if (curr_tkn)
	{
		if (lvl < 3)
		{
			create_leaves(shell, node, curr_tkn, start_end_tokens);
			return (OK);
		}
		if (is_subshell_start(start_end_tokens[0]))
			return (create_node_for_subshell(shell, node, start_end_tokens[0],
					start_end_tokens[1]));
		shell_node = create_shell_node(NODE_CMD, NULL);
		(*node)->set_content(*node, shell_node);
		shell_node->data.cmd = create_cmd_from_tokens(shell,
				start_end_tokens[0], start_end_tokens[1]);
		return (OK);
	}
	if (lvl >= 3 && is_subshell_start(start_end_tokens[0]))
		return (create_node_for_subshell(shell, node, start_end_tokens[0],
				start_end_tokens[1]));
	return (NO);
}

t_ast_node	*build_ast(t_shell *shell, t_ast_node **node, t_token *start_tkn,
		t_token *end_tkn)
{
	t_token	*start_end_tokens[2];

	start_end_tokens[0] = start_tkn;
	start_end_tokens[1] = end_tkn;
	if (*node == NULL)
		*node = create_ast_node(NULL);
	if (create_ast_node_for_lvl(shell, node, start_end_tokens, 1) == 1)
		return (*node);
	if (create_ast_node_for_lvl(shell, node, start_end_tokens, 2) == 1)
		return (*node);
	if (create_ast_node_for_lvl(shell, node, start_end_tokens, 3) == 1)
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
