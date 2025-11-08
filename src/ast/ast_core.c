/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:45:07 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/07 15:45:09 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

static void	free_ast_node(t_ast_node *node, void (*free_content)(void *))
{
	if (!node)
		return ;
	if (node->content)
		free_content(node->content);
	if (node->left)
		free_ast_node(node->left, free_content);
	if (node->right)
		free_ast_node(node->right, free_content);
	free(node);
}

static void	free_ast(t_ast *ast, void (*free_content)(void *))
{
	if (!ast)
		return ;
	free_ast_node(ast->root, free_content);
	free(ast);
}

static void	set_root(t_ast *ast, t_ast_node *root)
{
	if (!ast)
		return ;
	ast->root = root;
}

static t_ast_node	*get_root(t_ast *ast)
{
	if (!ast)
		return (NULL);
	return (ast->root);
}

t_ast	*create_ast_tree(void)
{
	t_ast	*ast;

	ast = malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->root = NULL;
	ast->free = free_ast;
	ast->set_root = set_root;
	ast->get_root = get_root;
	return (ast);
}
