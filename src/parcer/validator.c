/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:33:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/25 22:12:57 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "error.h"
#include "libft.h"
#include "validator_internal.h"

static int	syntax_error_symbol(char *symbol)
{
	char	*msg;

	msg = ft_strdup(SYNTAX_ERROR_UNEXPECTED_TOKEN);
	ft_strappend(&msg, symbol);
	ft_strappend(&msg, "'");
	output_error(NULL, msg);
	free(msg);
	return (0);
}

int	syntax_error_token(t_token *token)
{
	return (syntax_error_symbol((char *)get_token_str(token)));
}

static int	syntax_error_eof(void)
{
	output_error(SYNTAX_ERROR, SYNTAX_ERROR_UNEXPECTED_EOF);
	return (0);
}

static int	check_token_errors(t_token *token)
{
	if (token->error == OK)
		return (syntax_error_symbol("quote"));
	return (OK);
}

int	validate_tokens(t_token *start)
{
	t_token	*curr;
	t_token	*prev;
	int		paren_depth;

	if (!start || start->type == TOKEN_END)
		return (1);
	curr = start;
	prev = NULL;
	paren_depth = 0;
	while (curr && curr->type != TOKEN_END)
	{
		paren_depth = check_paren_balance(curr, paren_depth);
		if (paren_depth < 0)
			return (syntax_error_token(curr));
		if (!check_empty_parens(curr) || !check_redir_target(curr)
			|| !check_operators(curr, prev) || !check_paren_place(curr, prev)
			|| !check_token_errors(curr))
			return (NO);
		prev = curr;
		curr = curr->next;
	}
	if (paren_depth > 0 || (prev && is_token_operator(prev)
			&& prev->type != TOKEN_SEMICOLON))
		return (syntax_error_eof());
	return (OK);
}
