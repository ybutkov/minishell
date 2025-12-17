/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 20:20:19 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/17 20:35:46 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "lexer_internal.h"

void	free_token(t_token *token)
{
	t_piece	*cur;
	t_piece	*temp;
	
	free(token->value);
	if (token->pieces)
	{
		cur = token->pieces;
		while (cur)
		{
			temp = cur->next;
			free(cur->text);
			free(cur);
			cur = temp;
		}
	}
	free(token);
}

void	free_tokens(t_token *tokens)
{
	t_token	*cur;
	t_token	*temp;

	cur = tokens;
	while (cur)
	{
		temp = cur->next;
		// free_token(cur);
		cur->free(cur);
		cur = temp;
	}
}
