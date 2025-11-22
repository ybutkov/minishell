/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:36:35 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/21 16:23:10 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL
# define LEXER_INTERNAL

# include "parsing.h"
#include <unistd.h>

typedef struct	s_lex_inf
{
	char	*line;
	int		i;
	int		start;
	int		end;
	t_token	*head;
	t_token *tail;
	int		error_code;

}	t_lex_inf;

//lexer
t_token	*lexicalization(char *line);
void	is_space(t_lex_inf *lex);
void	quotes_within(t_lex_inf *lex, char quotes);
void	is_single_quote(t_lex_inf *lex);
void	is_double_quote(t_lex_inf *lex);
void	lex_struct_init(char *line, t_lex_inf *lex);

//tokenizer
t_token	*new_token(t_lex_inf *lex);
void	push_token(t_lex_inf *lex, t_token *tok);
void	type_of_token(t_token *tok);

//utils
int		is_whitespace(char *c);

#endif