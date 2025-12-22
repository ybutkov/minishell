/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:36:35 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/23 00:34:40 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "parsing.h"
# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_lex_inf
{
	char	*line;
	int		i;
	int		start;
	int		end;
	t_token	*head;
	t_token	*tail;
	int		error_code;

}	t_lex_inf;

typedef enum s_mix
{
	ALL_CLOSED = 0,
	STILL_DOUBLE = 1,
	STILL_SINGLE = 2,
}	t_mix;

typedef struct s_pieces_internal
{
	int		i;
	int		cur_start;
	int		cur_end;
	t_mix	q_stat;
	char	quotes;
}	t_pieces_internal;

typedef struct s_preproc
{
	char	*res;
	int		i;
	int		j;
	int		res_len;
	int		single_opened;
	int		double_opened;
	int		delta_size;
}	t_preproc;

// preprocessor
char	*preprocessing(char *str);
//lexer
t_token	*read_and_lexicalize(void);
t_token	*lexicalization(char *line);
void	lex_struct_init(char *line, t_lex_inf *lex);

//spaces & quotes
void	is_space(t_lex_inf *lex);
void	quotes_within(t_lex_inf *lex);
void	is_single_quote(t_lex_inf *lex);
void	is_double_quote(t_lex_inf *lex);
void	is_operator(t_lex_inf *lex);

//tokenizer
void	new_token(t_lex_inf *lex, e_quotes_status status);
void	simple_value(t_lex_inf *lex, t_token *tok);
void	push_token(t_lex_inf *lex, t_token *tok);
void	type_of_token(t_token *tok);
void	end_token(t_lex_inf *lex);
void	check_mixed(t_token *tok);

//mixed token values
void	mixed_value_assign(t_token *t);
void	assign_s_quo_pieces(t_token *t, t_pieces_internal *pi);
void	assign_d_quo_pieces(t_token *t, t_pieces_internal *pi);
void	new_piece(t_token *t, t_pieces_internal *pi, e_quotes_status q);
void	decide_on_extra(t_piece *p);
void	decide_on_extra_in_token(t_token *t);
void	extras_token(t_token *t);
void	push_piece(t_token *t, t_piece *p);
void	assign_env_wild_pieces(t_token *t, t_pieces_internal *pi);
void	small_env_var_check(t_piece *p);
void	small_tilde_check(t_piece *p);
void	dollar_sign_assign(t_token *t, t_pieces_internal *pi);

//utils
int		cust_strchr(char c, char *str);
void	decide_on_quotes(t_lex_inf *l, t_mix *q, char *quotes);
void	initialize_pieces(t_pieces_internal *pieces);
// change token_init with init_token or create_token
void	token_init(t_token *tok);
void	init_token(t_token *tok, t_token_type type);

//symbol identification
int		is_whitespace(char c);
int		is_all_whitespace(char *str);
int		is_whitespace_or_special(char c);
int		is_special(t_lex_inf *lex);
int		is_space_or_quotes(char c);
int		will_end_env_var(char c);

//freeing
void	free_token(t_token *token);
void	free_tokens(t_token *tokens);

#endif
