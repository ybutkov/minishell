/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_bools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:33:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 03:20:00 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_token_type(t_token *token, t_token_type *types)
{
	if (!token)
		return (0);
	return (token->type == types[0]
		|| token->type == types[1]
		|| token->type == types[2]
		|| token->type == types[3]);
}

int	is_token_operator(t_token *token)
{
	return (is_token_type(token, (t_token_type[]){TOKEN_PIPE, TOKEN_OR,
			TOKEN_AND, TOKEN_SEMICOLON}));
}

int	is_redir(t_token *token)
{
	return (is_token_type(token, (t_token_type[]){TOKEN_REDIR_IN,
			TOKEN_REDIR_OUT, TOKEN_REDIR_APPEND, TOKEN_HEREDOC}));
}
