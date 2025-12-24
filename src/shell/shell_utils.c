/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 18:49:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 21:46:43 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell_internal.h"

int	return_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}

static t_token_lvl	*create_token_lvl(int lvl)
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

t_token_lvl	*get_token_lvl(int lvl)
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

int	check_paren(t_token **curr_token, int *opened)
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

void	free_shell_node_bridge(void *content)
{
	t_shell_node	*node;

	if (!content)
		return ;
	node = (t_shell_node *)content;
	if (node && node->free)
		node->free(node);
}
