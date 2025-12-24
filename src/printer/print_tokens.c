/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:36:33 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 17:24:15 by ashadrin         ###   ########.fr       */
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

static void	print_token_pieces(t_piece *p, int env_present)
{
	int	pi;

	(void)env_present;
	pi = 0;
	while (p)
	{
		printf(" piece[%d] q=%d $=%d *=%d ~=%d text='",
			pi, (int)p->quotes, p->has_env_v, p->has_wild, p->has_tilde);
		if (p->text)
			printf("%s", p->text);
		else
			printf("(null)");
		printf("'\n");
		p = p->next;
		++pi;
	}
}

void	print_tokens_brief_once(t_token *toks, t_env *env)
{
	t_token	*t;
	int		ti;

	get_token_type_name(5);
	if (!toks)
		return ;
	ti = 0;
	t = toks;
	while (t)
	{
		printf("Token[%2d]: %-20s quotes:%-3d '",
			ti, get_token_type_name(t->type), t->stat);
		if (t->value)
			printf("%s", t->value);
		else
			printf("(null)");
		printf("'\n");
		if (t->pieces && env)
			print_token_pieces(t->pieces, 1);
		t = t->next;
		++ti;
	}
}
		// char **words = expand_and_split_token(t, env, 0);
		// if (!words)
		// 	continue ;
		// printf("%s", "-->");
		// for (int i = 0; words[i]; i++)
		// {
		// 	if (i != 0)
		// 		printf(",");
		// 	printf("%s", words[i]);
		// }
		// printf("%s", "<--\n");
		// free_str_array(words);
