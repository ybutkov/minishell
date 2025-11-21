/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enams.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:09:28 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/21 19:09:31 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_node.h"
#include "parcer.h"

static void	init_node_type_mapping(t_node_type *node_type_map)
{
	node_type_map[TOKEN_AND] = NODE_AND;
	node_type_map[TOKEN_OR] = NODE_OR;
	node_type_map[TOKEN_SEMICOLON] = NODE_SEMICOLON;
	node_type_map[TOKEN_PIPE] = NODE_PIPE;
	node_type_map[TOKEN_REDIR_IN] = NODE_REDIR_IN;
	node_type_map[TOKEN_REDIR_OUT] = NODE_REDIR_OUT;
	node_type_map[TOKEN_REDIR_APPEND] = NODE_REDIR_APPEND;
	node_type_map[TOKEN_HEREDOC] = NODE_REDIR_HEREDOC;
}

t_node_type	get_node_type_by_token(t_token_type token_type)
{
	static t_node_type	node_type[13];

	if (node_type[TOKEN_AND] == 0)
		init_node_type_mapping(node_type);
	return (node_type[token_type]);
}
