/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build_subshell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:53:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 21:43:38 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "libft.h"
#include "parsing.h"
#include "shell_internal.h"
#include "utils.h"

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

int	is_subshell_start(t_token *start_tkn)
{
	t_token	*closed_paren;

	if (!start_tkn || start_tkn->type != TOKEN_LEFT_PAREN)
		return (0);
	closed_paren = find_paren(start_tkn);
	return (closed_paren != NULL);
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
