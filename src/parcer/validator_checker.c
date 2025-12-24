/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_checker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:33:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 03:41:30 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator_internal.h"

int	check_paren_balance(t_token *curr, int paren_depth)
{
	if (curr->type == TOKEN_LEFT_PAREN)
		return (paren_depth + 1);
	if (curr->type == TOKEN_RIGHT_PAREN)
	{
		if (paren_depth <= 0)
			return (-1);
		return (paren_depth - 1);
	}
	return (paren_depth);
}

int	check_empty_parens(t_token *curr)
{
	if (curr->type == TOKEN_LEFT_PAREN && curr->next
		&& curr->next->type == TOKEN_RIGHT_PAREN)
		return (syntax_error_token(curr->next));
	else
		return (1);
}

int	check_redir_target(t_token *curr)
{
	if (is_redir(curr))
	{
		if (!curr->next || curr->next->type == TOKEN_END
			|| is_token_operator(curr->next) || is_redir(curr->next)
			|| curr->next->type == TOKEN_LEFT_PAREN
			|| curr->next->type == TOKEN_RIGHT_PAREN)
		{
			if (curr->next && curr->next->type != TOKEN_END)
				return (syntax_error_token(curr->next));
			else
				return (syntax_error_token(NULL));
		}
	}
	return (1);
}

int	check_operators(t_token *curr, t_token *prev)
{
	if (!prev && is_token_operator(curr))
		return (syntax_error_token(curr));
	if (prev && is_token_operator(prev) && is_token_operator(curr))
		return (syntax_error_token(curr));
	return (1);
}

int	check_paren_place(t_token *curr, t_token *prev)
{
	if (curr->type == TOKEN_LEFT_PAREN && prev && prev->type == TOKEN_WORD)
		return (syntax_error_token(curr));
	return (1);
}
