/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:45:19 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/07 15:45:21 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ast_node_internal.h"
#include <stdlib.h>

static t_ast_node	*get_left(t_ast_node *node)
{
	if (!node)
		return (NULL);
	return (node->left);
}

static t_ast_node	*get_right(t_ast_node *node)
{
	if (!node)
		return (NULL);
	return (node->right);
}

static void	set_left(t_ast_node *node, t_ast_node *left)
{
	if (!node)
		return ;
	node->left = left;
}

static void	set_right(t_ast_node *node, t_ast_node *right)
{
	if (!node)
		return ;
	node->right = right;
}

t_ast_node	*create_ast_node(void *content)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->content = content;
	node->left = NULL;
	node->right = NULL;
	node->get_left = get_left;
	node->get_right = get_right;
	node->set_left = set_left;
	node->set_right = set_right;
	node->get_content = get_content;
	node->set_content = set_content;
	return (node);
}
