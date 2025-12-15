/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixed_value_assign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:45:44 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/15 14:45:08 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	mixed_value_assign(t_lex_inf *l, t_token *t)
{
	t_pieces_internal	pi;

	pi.i = 0;
	initialize_pieces(&pi);
	while (t->value[pi.i] != '\0')
	{
		if (t->value[pi.i] == '"')
			assign_d_quo_pieces(l, t, &pi);
		else if (t->value[pi.i] == '\'')
			assign_s_quo_pieces(l, t, &pi);
		else if (t->value[pi.i] == '$' || t->value[pi.i] == '*' || t->value[pi.i] == '~')
			assign_env_wild_pieces(l, t, &pi);
		else
		{
			pi.cur_start = pi.i;
			while (is_whitespace(t->value[pi.i]))
				pi.i++;
			while (!is_space_or_quotes(t->value[pi.i]) && t->value[pi.i] != '\0'
					&& t->value[pi.i] != '*' && t->value[pi.i] != '$'
					&& t->value[pi.i] != '\'')
				pi.i++;
			pi.cur_end = pi.i - 1;
			if (pi.cur_end >= pi.cur_start)
				new_piece(t, &pi, l, NO_QUOTES);
			if (t->value[pi.i] == '\'')
			{
				pi.q_stat = ALL_CLOSED;
				assign_s_quo_pieces(l, t, &pi);
			}
		}
		// else
		// 	pi.i++;
	}
}

void	assign_s_quo_pieces(t_lex_inf *l, t_token *t, t_pieces_internal *pi)
{
	pi->i++;
	pi->cur_start = pi->i;
	while (t->value[pi->i] != '\'' && t->value[pi->i] != '\0')
		pi->i++;
	pi->cur_end = pi->i - 1;
	new_piece(t, pi, l, SINGLE_Q);
	if (t->value[pi->i] == '\'')
		pi->i++;
}

void	assign_d_quo_pieces(t_lex_inf *l, t_token *t, t_pieces_internal *pi)
{
	int	starting_i;
	
    pi->i++;
	starting_i = pi->i;
    pi->q_stat = STILL_DOUBLE;
    
    while (t->value[pi->i] != '"' && t->value[pi->i] != '\0')
    {
        if (t->value[pi->i] == '$')
        {
            assign_env_wild_pieces(l, t, pi);
            continue;
        }
        pi->cur_start = pi->i;
        while (t->value[pi->i] != '$' && t->value[pi->i] != '"' 
                && t->value[pi->i] != '\0')
            pi->i++;
        pi->cur_end = pi->i - 1;
        if (pi->cur_end >= pi->cur_start)
            new_piece(t, pi, l, DOUBLE_Q);
    }
    if (pi->i == starting_i)
	{
		pi->cur_start = pi->i;
		pi->cur_end = pi->i - 1;
		new_piece(t, pi, l, DOUBLE_Q);
	}
    if (t->value[pi->i] == '"')
        pi->i++;
    pi->q_stat = ALL_CLOSED;
}


// void	assign_d_quo_pieces(t_lex_inf *l, t_token *t, t_pieces_internal *pi)
// {
// 	pi->q_stat = STILL_DOUBLE;
// 	pi->i++;
// 	while (t->value[pi->i] != '"' && t->value[pi->i] != '\0')
// 	{
// 		if (t->value[pi->i] == '$')
// 		{
// 			pi->cur_start = pi->i;
// 			while (is_space_or_quotes(t->value[pi->i]) && t->value[pi->i] != '\0')
// 				pi->i++;
// 			pi->cur_end = pi->i - 1;
// 			new_piece(t, pi, l, DOUBLE_QUOTE);
// 		}
// 		if (t->value[pi->i] == '$')
// 		{
// 			pi->cur_start = pi->i;
// 			while (!is_whitespace(t->value[pi->i]) && t->value[pi->i] != '"' && t->value[pi->i] != '\0')
// 				pi->i++;
// 			pi->cur_end = pi->i - 1;
// 			new_piece(t, pi, l, DOUBLE_QUOTE);
		
// 		}
// 	}
// }

// void	assign_quoted_pieces(char quote, t_lex_inf *l, t_token *t, t_pieces_internal *pi)
// {
//     pi->i++; 
    
//     if (quote == '\'')
//     {
//         // Single quotes: one piece, everything literal
//         pi->cur_start = pi->i;
//         while (t->value[pi->i] != '\'' && t->value[pi->i] != '\0')
//             pi->i++;
//         pi->cur_end = pi->i - 1;
//         if (pi->cur_end >= pi->cur_start)
//             new_piece(t, pi, l, SINGLE_Q);
//         if (t->value[pi->i] == '\'')
//             pi->i++;
//         return;
//     }

//     while (t->value[pi->i] != '"' && t->value[pi->i] != '\0')
//     {
//         if (t->value[pi->i] == '$')
//         {
//             assign_env_wild_pieces(l, t, pi); 
//             continue;
//         }
//         pi->cur_start = pi->i;
//         while (t->value[pi->i] != '$' && t->value[pi->i] != '"' 
//                 && t->value[pi->i] != '\0')
//             pi->i++;
//         pi->cur_end = pi->i - 1;
//         if (pi->cur_end >= pi->cur_start)
//             new_piece(t, pi, l, DOUBLE_Q);
//     }
    
//     if (t->value[pi->i] == '"')
//         pi->i++;
// }

void	new_piece(t_token *t, t_pieces_internal *pi, t_lex_inf *lex, e_quotes_status q)
{
	t_piece	*p;
	int		len;

	p = malloc(sizeof(t_piece));
	if (!p)
	{
		lex->error_code = 2;
		return ;
	}
	len = pi->cur_end - pi->cur_start + 1;
	p->text = malloc(sizeof(char) * (len + 1));
	if (!p->text)
	{
		free(p);
		lex->error_code = 2;
		return ;
	}
	ft_memcpy(p->text, t->value + pi->cur_start, len);
	p->text[len] = '\0';
	p->quotes = q;
	p->next = NULL;
	p->has_tilde = 0;
	p->has_wild = 0;
	p->has_env_v = 0;
	decide_on_extra(p);
	push_piece(t, p);
}

void	assign_env_wild_pieces(t_lex_inf *l, t_token *t, t_pieces_internal *pi)
{
	pi->cur_start = pi->i;
	pi->i++;
	if (t->value[pi->cur_start] == '$')
	{
		while (!will_end_env_var(t->value[pi->i]))
			pi->i++;
	}
	else
	{
		while (!is_space_or_quotes(t->value[pi->i]) && t->value[pi->i] != '\0' && t->value[pi->i] != '$')
			pi->i++;
	}
	pi->cur_end = pi->i - 1;
	if (pi->q_stat == STILL_DOUBLE)
		new_piece(t, pi, l, DOUBLE_Q);
	else
		new_piece(t, pi, l, NO_QUOTES);
}

void	decide_on_extra(t_piece *p)
{
	int	i;
	
	if (!p || !p->text)
		return ;
	i = 0;
	p->has_wild = 0;
	p->has_env_v = 0;
	if (p->quotes == SINGLE_Q)
		return ;
	while (p->text[i])
	{
		if (p->text[i] == '~' && p->quotes == NO_QUOTES)
			small_tilde_check(p);
		if (p->text[i] == '*' && p->quotes == NO_QUOTES)
			p->has_wild = 1;
		if (p->text[i] == '$') //don't know if there are any other specific conditions yet
			small_env_var_check(p);
		i++;
	}
}
void	small_tilde_check(t_piece *p)
{
	int	i;

	if (p->text[0] == '~' && (p->text[1] == '\0' || p->text[1] == '/'))
	{
		p->has_tilde = 1;
		return ;
	}
	i = 1;
	while (p->text[i])
	{
		if (p->text[i] == '~' && (p->text[i - 1] == ':' || p->text[i - 1] == '='))
		{
			if (p->text[i + 1] == '\0' || p->text[i + 1] == '/')
			{
				p->has_tilde = 1;
				return ;
			}
		}
		i++;	
	}
}

void	small_env_var_check (t_piece *p)
{
	int	i;

	if (!p || p->text[0] != '$')
		return ;
	if (!ft_isalpha(p->text[1]) && p->text[1] != '_' && p->text[1] != '?')
		return ;
	if (p->text[1] == '?')
	{
		p->has_env_v = 1;
		return ;
	}
	i = 2;
	while (p->text[i])
	{
		if (!ft_isalnum(p->text[i]) && p->text[i] != '_')
			return ;
		i++;
	}
	p->has_env_v = 1;
}

void	decide_on_extra_in_token(t_token *t)
{
	t_piece	*node;
	
	if (t->pieces)
	{
		node = t->pieces;
		while (node)
		{
			if (node->has_wild)
				t->has_wild = 1;
			if (node->has_env_v)
				t->has_env_v = 1;
			node = node->next;
		}
	}
	else if (t->stat == NO_QUOTES || t->stat == DOUBLE_Q)
		extras_token(t);
}

void	extras_token(t_token *t)
{
	int	i;
	
	i = 0;
	while (t->value[i])
	{
		if (t->value[i] == '*' && t->stat == NO_QUOTES)
			t->has_wild = 1;
		if (t->value[i] == '$')
			t->has_env_v = 1;
		i++;		
	}
}

void	push_piece(t_token *t, t_piece *p)
{
	if (!t || !p)
		return ;
	if (!t->pieces)
	{
		t->pieces = p;
		t->pieces_tail = p;
		return ;
	}
	t->pieces_tail->next = p;
	t->pieces_tail = p;
	p->next = NULL;
}


// void	assign_quoted_pieces(char quote, t_lex_inf *l, t_token *t, t_pieces_internal *pi)
// {
// 	pi->i++;
// 	if (t->value[pi->i] == quote)
// 	{
// 		pi->i++;
// 		return ;
// 	}
// 	pi->cur_start = pi->i;
// 	if (quote == '"')
// 	{
// 		if (pi->q_stat = STILL_DOUBLE)
// 		{
			
// 		}
// 		pi->q_stat = STILL_DOUBLE;
// 		while (t->value[pi->i] != quote && t->value[pi->i] != '$'
// 				&& t->value[pi->i] != '\0')
// 			pi->i++;
// 	}
// 	else if (quote == '\'')
// 	{
// 		while (t->value[pi->i] != quote
// 				&& t->value[pi->i] != '\0')
// 			pi->i++;
// 	}
// 	pi->cur_end = pi->i - 1;
// 	if (quote == '\'')
// 		new_piece(t, pi, l, SINGLE_Q);
// 	else if (t->value[pi->i] == '$')
// 	{
// 		new_piece(t, pi, l, DOUBLE_Q);
// 		assign_env_wild_pieces(l, t, pi);
// 		return ;
// 	}
// 	else if (t->value[pi->i] == '"')
// 	{
// 		pi->q_stat = ALL_CLOSED;
// 		new_piece(t, pi, l, DOUBLE_Q);
// 	}
// 	pi->i++;
// }