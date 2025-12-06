/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:36:33 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/05 23:14:59 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>

void	print_tokens_brief_once(t_token *toks)
{
	t_token	*t;
	t_piece	*p;
	int		ti;
	int		pi;

	if (!toks)
	{
		// printf("no tokens\n");
		return ;
	}
	ti = 0;
	for (t = toks; t; t = t->next, ++ti)
	{
		printf("Token[%d]: type=%u '%s'\n", ti, t->type,
			t->value ? t->value : "(null)");
		if (!t->pieces)
			continue ;
		pi = 0;
		for (p = t->pieces; p; p = p->next, ++pi)
			printf("  piece[%d] q=%d text='%s'\n", pi, (int)p->quotes,
				p->text ? p->text : "(null)");
	}
}
