/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_bools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:33:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/25 22:05:03 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

const char	*get_token_str(t_token *token)
{
	if (!token)
		return ("newline");
	if (token->type == TOKEN_PIPE || token->type == TOKEN_OR
		|| token->type == TOKEN_AND || token->type == TOKEN_SEMICOLON
		|| token->type == TOKEN_LEFT_PAREN || token->type == TOKEN_RIGHT_PAREN
		|| token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_APPEND || token->type == TOKEN_HEREDOC)
		return (token->value);
	return ("newline");
}

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
