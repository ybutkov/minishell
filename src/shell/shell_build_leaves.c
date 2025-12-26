/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build_leaves.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:53:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/26 02:50:06 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "parsing.h"
#include "shell_internal.h"
#include "utils.h"

static t_token	*get_for_last(t_token *curr_token, t_token_lvl *token_lvl,
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

t_token	*get_next_token_for_lvl(t_token *curr, t_token *end_token,
		t_token_lvl *token_lvl)
{
	t_token	*found_token;
	int		subshell_opened;

	if (!token_lvl || token_lvl->start == (t_token_type)(-1))
		return (free(token_lvl), HANDLE_ERROR_NULL);
	found_token = NULL;
	subshell_opened = 0;
	while (curr != end_token)
	{
		if (check_paren(&curr, &subshell_opened) == 1)
			continue ;
		if (!subshell_opened && curr && curr->type >= token_lvl->start
			&& curr->type <= token_lvl->end)
		{
			if (token_lvl->lvl == 1)
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

static void	build_left_leaf(t_shell *shell, t_ast_node **node, t_token **start,
	t_token **end)
{
	t_token	*ls;
	t_token	*le;

	ls = *start;
	le = *end;
	build_ast(shell, node, &ls, &le);
	*start = ls;
}

static void	build_right_leaf(t_shell *shell, t_ast_node **node, t_token **start,
	t_token **end)
{
	t_token	*rs;
	t_token	*re;

	rs = *start;
	re = *end;
	build_ast(shell, node, &rs, &re);
	*end = re;
}

static void	build_right_leaf_for_pipe(t_shell *shell, t_ast_node **node, t_token **start,
	t_token **end)
{
	t_token			*rs;
	t_token			*re;
	// t_token			*possible_tkn;
	t_shell_node	*shell_node;

	// possible_tkn = get_next_token_for_lvl(start, end, get_token_lvl(2));

	shell_node = create_shell_node(NODE_PIPE, NULL);
	(*node)->set_content(*node, shell_node);

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
	int				pre_pipe;

	pre_pipe = 1;
	if ((*node)->get_content(*node) == NULL)
	{
		shell_node = create_shell_node(get_node_type(curr_tkn->type), NULL);
		(*node)->set_content(*node, shell_node);
		pre_pipe = 0;
	}
	left_node = create_ast_node(NULL);
	(*node)->set_left(*node, left_node);
	if (pre_pipe && curr_tkn->type != TOKEN_PIPE)
	{
		build_left_leaf(shell, &left_node, &start_end_tokens[0], &start_end_tokens[1]);
		pre_pipe = 0;
	}
	else
	{
		right_node = create_ast_node(NULL);
		(*node)->set_right(*node, right_node);
		build_left_leaf(shell, &left_node, &start_end_tokens[0], &curr_tkn->prev);
		if (curr_tkn->type == TOKEN_PIPE)
			build_right_leaf_for_pipe(shell, &right_node, &curr_tkn->next, &start_end_tokens[1]);
		else
			build_right_leaf(shell, &right_node, &curr_tkn->next, &start_end_tokens[1]);
	}
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
