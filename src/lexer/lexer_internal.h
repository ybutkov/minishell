/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:36:35 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/22 18:08:44 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL
# define LEXER_INTERNAL

# include "parsing.h"
#include <unistd.h>
#include <stdlib.h>

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
void	new_token(t_lex_inf *lex, e_quotes_status status);
void	push_token(t_lex_inf *lex, t_token *tok);
void	type_of_token(t_token *tok);

//utils
int		is_whitespace(char c);

#endif

