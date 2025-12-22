/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assigning_pieces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:59:49 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/22 22:12:43 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	assign_s_quo_pieces(t_token *t, t_pieces_internal *pi)
{
	pi->i++;
	pi->cur_start = pi->i;
	while (t->value[pi->i] != '\'' && t->value[pi->i] != '\0')
		pi->i++;
	pi->cur_end = pi->i - 1;
	if (pi->cur_end >= pi->cur_start)
		new_piece(t, pi, SINGLE_Q);
	pi->i++;
}

void	assign_d_quo_pieces(t_token *t, t_pieces_internal *pi)
{
	int	starting_i;

	pi->i++;
	starting_i = pi->i;
	pi->q_stat = STILL_DOUBLE;
	while (t->value[pi->i] != '"' && t->value[pi->i] != '\0')
	{
		if (t->value[pi->i] == '$')
		{
			assign_env_wild_pieces(t, pi);
			continue ;
		}
		dollar_sign_assign(t, pi);
	}
	if (pi->i == starting_i)
	{
		pi->cur_start = pi->i;
		pi->cur_end = pi->i - 1;
		new_piece(t, pi, DOUBLE_Q);
	}
	if (t->value[pi->i] == '"')
		pi->i++;
	pi->q_stat = ALL_CLOSED;
}

void	dollar_sign_assign(t_token *t, t_pieces_internal *pi)
{
	pi->cur_start = pi->i;
	while (t->value[pi->i] != '$' && t->value[pi->i] != '"'
		&& t->value[pi->i] != '\0')
		pi->i++;
	pi->cur_end = pi->i - 1;
	if (pi->cur_end >= pi->cur_start)
		new_piece(t, pi, DOUBLE_Q);
}

void	assign_env_wild_pieces(t_token *t, t_pieces_internal *pi)
{
	pi->cur_start = pi->i;
	pi->i++;
	if (t->value[pi->cur_start] == '$')
	{
		if (ft_isalpha(t->value[pi->cur_start + 1])
			|| t->value[pi->cur_start + 1] == '_'
			|| t->value[pi->cur_start + 1] == '?')
		{
			pi->i++;
			while (!will_end_env_var(t->value[pi->i]))
				pi->i++;
		}
	}
	else
	{
		while (!is_space_or_quotes(t->value[pi->i])
			&& t->value[pi->i] != '\0' && t->value[pi->i] != '$')
			pi->i++;
	}
	pi->cur_end = pi->i - 1;
	if (pi->q_stat == STILL_DOUBLE)
		new_piece(t, pi, DOUBLE_Q);
	else
		new_piece(t, pi, NO_QUOTES);
}
