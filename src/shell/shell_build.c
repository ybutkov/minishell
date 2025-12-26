/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:53:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/25 23:03:06 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "libft.h"
#include "parsing.h"
#include "shell_internal.h"
#include "utils.h"

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

int	create_ast_node_for_lvl(t_shell *shell, t_ast_node **node,
		t_token **start_end_tokens, int lvl)
{
	t_token			*curr_tkn;
	t_shell_node	*shell_node;

	curr_tkn = get_next_token_for_lvl(start_end_tokens[0], start_end_tokens[1],
			get_token_lvl(lvl));
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
		if ((*node)->get_content(*node) != NULL)
		{
			create_leaves(shell, node, curr_tkn, start_end_tokens);
			return (OK);
		}
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
