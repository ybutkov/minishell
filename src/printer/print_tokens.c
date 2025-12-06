/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:36:33 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/06 23:57:55 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>

static const char	*get_token_type_name(t_token_type type)
{
	static const char	*names[] = {
		"",
		"TOKEN_AND",
		"TOKEN_OR",
		"TOKEN_SEMICOLON",
		"TOKEN_PIPE",
		"TOKEN_WORD",
		"TOKEN_REDIR_IN",
		"TOKEN_REDIR_OUT",
		"TOKEN_REDIR_APPEND",
		"TOKEN_HEREDOC",
		"TOKEN_NEWLINE",
		"TOKEN_LEFT_PAREN",
		"TOKEN_RIGHT_PAREN",
		"TOKEN_END",
		"TOKEN_WILDCARD"
	};

	if (type >= 0 && type <= TOKEN_WILDCARD)
		return (names[type]);
	return ("UNKNOWN");
}

void	print_tokens_brief_once(t_token *toks)
{
	t_token	*t;
	t_piece	*p;
	int		ti;
	int		pi;

	get_token_type_name(5);
	if (!toks)
	{
		// printf("no tokens\n");
		return ;
	}
	ti = 0;
	for (t = toks; t; t = t->next, ++ti)
	{
		printf("Token[%2d]: %-20s '%s'\n", ti, get_token_type_name(t->type),
			t->value ? t->value : "(null)");
		if (!t->pieces)
			continue ;
		pi = 0;
		for (p = t->pieces; p; p = p->next, ++pi)
			printf(" piece[%d] q=%d $=%d *=%d text='%s'\n", pi, (int)p->quotes,
				p->has_env_v, p->has_wild, p->text ? p->text : "(null)");
	}
}
