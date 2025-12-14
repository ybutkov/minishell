/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:36:33 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/14 02:21:01 by ashadrin         ###   ########.fr       */
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

void	print_tokens_brief_once(t_token *toks, t_env *env)
{
	t_token	*t;
	t_piece	*p;
	int		ti;
	int		pi;

	(void)env;
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
		char **words = expand_and_split_token(t, env, 0);
		printf("%s", "-->");
		for (int i = 0; words[i]; i++)
		{
			if (i != 0)
				printf(",");
			printf("%s", words[i]);
		}
		printf("%s", "<--\n");

		pi = 0;
		for (p = t->pieces; p; p = p->next, ++pi)
			printf(" piece[%d] q=%d $=%d *=%d ~=%d text='%s'\n",
       		pi, (int)p->quotes, p->has_env_v, p->has_wild, p->has_tilde,
       		p->text ? p->text : "(null)");
	}
}
