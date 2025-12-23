/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:53:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 05:06:22 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "libft.h"
#include "parsing.h"
#include "shell_internal.h"
#include "utils.h"

t_ast_node		*build_ast(t_shell *shell, t_ast_node **node,
					t_token **start_tkn, t_token **end_tkn);

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

int	collect_exp_args_to_list(char **expanded_args, char **new_arg,
	t_list **arg_list)
{
	int		i;

	i = -1;
	while (expanded_args[++i])
	{
		if (ft_strcmp(expanded_args[i], " ") == 0)
		{
			ft_lstadd_back(arg_list, ft_lstnew(*new_arg));
			*new_arg = NULL;
		}
		else if (ft_strappend(new_arg, expanded_args[i]) == 0)
		{
			return (NO);
		}
	}
	return (OK);
}

int	collect_pieces_to_strings(t_shell *shell, t_token *curr_tkn,
	t_list **arg_list)
{
	char	**expanded_args;
	char	*new_arg;

	new_arg = NULL;
	if (curr_tkn->pieces)
	{
		expanded_args = expand_and_split_token(curr_tkn, shell->ctx->env,
				shell->ctx->last_exit_status);
		if (!expanded_args)
			return (free(expanded_args), NO);
		if (collect_exp_args_to_list(expanded_args, &new_arg, arg_list) == NO)
			return (NO);
		if (new_arg)
			ft_lstadd_back(arg_list, ft_lstnew(new_arg));
		free_str_array(expanded_args);
	}
	else
		ft_lstadd_back(arg_list, ft_lstnew(ft_strdup(curr_tkn->value)));
	return (OK);
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
	t_redir	*redirect;

	target = collect_pieces_to_string(shell, curr_tkn);
	if (target == NULL)
		return (NULL);
	redirect = create_redir(type, target, curr_tkn->stat);
	free(target);
	return (redirect);
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

t_token_lvl	*create_token_lvl(int lvl)
{
	t_token_lvl		*token_lvl;

	token_lvl = malloc(sizeof(t_token_lvl));
	if (token_lvl == NULL)
		return (HANDLE_ERROR_NULL);
	token_lvl->lvl = lvl;
	token_lvl->start = (t_token_type)(-1);
	token_lvl->end = (t_token_type)(-1);
	return (token_lvl);
}

static t_token_lvl	*get_token_lvl(int lvl)
{
	t_token_lvl		*token_lvl;

	token_lvl = create_token_lvl(lvl);
	if (token_lvl == NULL)
		return (HANDLE_ERROR_NULL);
	if (token_lvl->lvl == 1)
	{
		token_lvl->start = TKN_LVL_1_FROM;
		token_lvl->end = TKN_LVL_1_TO;
	}
	else if (token_lvl->lvl == 2)
	{
		token_lvl->start = TKN_LVL_2_FROM;
		token_lvl->end = TKN_LVL_2_TO;
	}
	else if (token_lvl->lvl == 3)
	{
		token_lvl->start = TKN_LVL_3_FROM;
		token_lvl->end = TKN_LVL_3_TO;
	}
	return (token_lvl);
}

static int	check_paren(t_token **curr_token, int *opened)
{
	if (!curr_token || !*curr_token)
		return (0);
	if ((*curr_token)->type == TOKEN_LEFT_PAREN)
	{
		*opened += 1;
		*curr_token = (*curr_token)->next;
		return (1);
	}
	if (*opened && (*curr_token)->type == TOKEN_RIGHT_PAREN)
	{
		*opened -= 1;
		*curr_token = (*curr_token)->next;
		return (1);
	}
	return (0);
}

t_token	*get_for_last(t_token *curr_token, t_token_lvl *token_lvl,
		t_token **found_token, int subshell_opened)
{
	if (curr_token->type >= token_lvl->start
		&& curr_token->type <= token_lvl->end && !subshell_opened)
	{
		if (token_lvl->lvl == 1)
			*found_token = curr_token;
		else
			return (free(token_lvl), curr_token);
	}
	return (free(token_lvl), *found_token);
}

t_token	*get_next_token_for_lvl(t_token *start_token, t_token *end_token,
		int lvl)
{
	t_token			*curr;
	t_token			*found_token;
	t_token_lvl		*token_lvl;
	int				subshell_opened;

	token_lvl = get_token_lvl(lvl);
	if (!token_lvl || token_lvl->start == (t_token_type)(-1))
		return (free(token_lvl), HANDLE_ERROR_NULL);
	curr = start_token;
	found_token = NULL;
	subshell_opened = 0;
	while (curr != end_token)
	{
		if (check_paren(&curr, &subshell_opened) == 1)
			continue ;
		if (!subshell_opened && curr && curr->type >= token_lvl->start
			&& curr->type <= token_lvl->end)
		{
			if (lvl == 1)
				found_token = curr;
			else
				return (free(token_lvl), curr);
		}
		curr = curr->next;
	}
	if (curr && curr == end_token)
		return (get_for_last(curr, token_lvl, &found_token, subshell_opened));
	return (free(token_lvl), found_token);
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

char	*get_raw_string(t_token *tkn)
{
	char	*quote;
	char	*raw_str;

	quote = NULL;
	raw_str = NULL;
	if (tkn->pieces)
		return (ft_strdup(tkn->value));
	if (tkn->stat == DOUBLE_Q)
		quote = "\"";
	else if (tkn->stat == SINGLE_Q)
		quote = "\'";
	if (quote)
		ft_strappend(&raw_str, quote);
	ft_strappend(&raw_str, tkn->value);
	if (quote)
		ft_strappend(&raw_str, quote);
	return (raw_str);
}

t_cmd	*create_cmd_from_tokens(t_shell *shell, t_token **start_tkn,
		t_token **end_tkn)
{
	t_cmd	*cmd;

	cmd = create_cmd(NULL, NULL);
	if (collect_redirs(shell, cmd, start_tkn, end_tkn) == ERROR)
		return (cmd->free_cmd(cmd), HANDLE_ERROR_NULL);
	cmd->tokens = copy_tokens(*start_tkn, *end_tkn);
	return (cmd);
}

int	create_node_for_subshell(t_shell *shell, t_ast_node **node,
	t_token *start_tkn, t_token *end_tkn)
{
	t_shell_node	*shell_node;
	t_ast_node		*left_node;
	t_token			*matching_paren;
	t_token			*start;
	t_token			*end;

	shell_node = create_shell_node(NODE_SUBSHELL, NULL);
	(*node)->set_content(*node, shell_node);
	left_node = create_ast_node(NULL);
	(*node)->set_left(*node, left_node);
	matching_paren = find_paren(start_tkn);
	if (matching_paren && matching_paren->next && matching_paren != end_tkn)
		collect_subshell_redirs(shell, shell_node, matching_paren->next,
			end_tkn->next);
	{
		start = start_tkn->next;
		end = matching_paren->prev;
		build_ast(shell, &left_node, &start, &end);
	}
	return (1);
}

void	build_left_leaf(t_shell *shell, t_ast_node **node, t_token **start,
	t_token **end)
{
	t_token	*ls;
	t_token	*le;

	ls = *start;
	le = *end;
	build_ast(shell, node, &ls, &le);
	*start = ls;
}

void	build_right_leaf(t_shell *shell, t_ast_node **node, t_token **start,
	t_token **end)
{
	t_token	*rs;
	t_token	*re;

	rs = *start;
	re = *end;
	build_ast(shell, node, &rs, &re);
	*end = re;
}

void	create_leaves(t_shell *shell, t_ast_node **node, t_token *curr_tkn,
	t_token **start_end_tokens)
{
	t_shell_node	*shell_node;
	t_ast_node		*left_node;
	t_ast_node		*right_node;
	t_token			*tmp;

	shell_node = create_shell_node(get_node_type(curr_tkn->type), NULL);
	(*node)->set_content(*node, shell_node);
	left_node = create_ast_node(NULL);
	right_node = create_ast_node(NULL);
	(*node)->set_left(*node, left_node);
	(*node)->set_right(*node, right_node);
	build_left_leaf(shell, &left_node, &start_end_tokens[0], &curr_tkn->prev);
	build_right_leaf(shell, &right_node, &curr_tkn->next, &start_end_tokens[1]);
	tmp = curr_tkn->next;
	if (curr_tkn->prev)
		curr_tkn->prev->next = tmp;
	if (tmp)
		tmp->prev = curr_tkn->prev;
	if (curr_tkn == start_end_tokens[0])
		start_end_tokens[0] = tmp;
	if (curr_tkn == start_end_tokens[1])
		start_end_tokens[1] = curr_tkn->prev;
	curr_tkn->free(curr_tkn);
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
				&start_end_tokens[0], &start_end_tokens[1]);
		return (OK);
	}
	if (lvl >= 3 && is_subshell_start(start_end_tokens[0]))
		return (create_node_for_subshell(shell, node, start_end_tokens[0],
				start_end_tokens[1]));
	return (NO);
}

t_ast_node	*build_ast(t_shell *shell, t_ast_node **node, t_token **start_tkn,
		t_token **end_tkn)
{
	t_token	*start_end_tokens[2];

	if (!start_tkn || !end_tkn || !*start_tkn || !*end_tkn)
		return (NULL);
	start_end_tokens[0] = *start_tkn;
	start_end_tokens[1] = *end_tkn;
	if (*node == NULL)
		*node = create_ast_node(NULL);
	if (create_ast_node_for_lvl(shell, node, start_end_tokens, 1) == 1)
	{
	}
	else if (create_ast_node_for_lvl(shell, node, start_end_tokens, 2) == 1)
	{
	}
	else if (create_ast_node_for_lvl(shell, node, start_end_tokens, 3) == 1)
	{
	}
	*start_tkn = start_end_tokens[0];
	*end_tkn = start_end_tokens[1];
	return (*node);
}

void	build_shell(t_shell *shell, t_token **token)
{
	t_ast_node	*root_node;
	t_token		*end_token;

	root_node = NULL;
	if (!token || !*token)
		return ;
	end_token = *token;
	while (end_token->next)
		end_token = end_token->next;
	end_token = end_token->prev;
	if (!validate_tokens(*token))
		return ;
	end_token->next->free(end_token->next);
	end_token->next = NULL;
	root_node = build_ast(shell, &root_node, token, &end_token);
	shell->ast->set_root(shell->ast, root_node);
}
