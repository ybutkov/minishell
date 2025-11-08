/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_core_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:45:13 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/07 15:45:15 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

void	*get_content(t_ast_node *node)
{
	if (!node)
		return (NULL);
	return (node->content);
}

void	*set_content(t_ast_node *node, void *content)
{
	if (!node)
		return (NULL);
	node->content = content;
	return (content);
}
