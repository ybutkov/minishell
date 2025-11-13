/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:52:42 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/12 12:59:01 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARCING_H
# define PARCING_H

# define SINGLE_QUOTE '\''
# define DOUNLE_QUOTE '\"'
# define QUOTES "\'\""
# define SPACES " \t\n\r\v\f"
# define SPECIAL_CHARS "<>|&"
# define SEMICOLON ';'
# define LEFT_PAREN '('
# define RIGHT_PAREN ')'
# define LESSER_THAN '<'
# define GREATER_THAN '>'
# define TKN_LVL_1_FROM 1
# define TKN_LVL_1_TO 3
# define TKN_LVL_2_FROM 4
# define TKN_LVL_2_TO 4
# define TKN_LVL_3_FROM 5
# define TKN_LVL_3_TO 12

typedef enum	s_token_type
{
	TOKEN_AND = 1,
	TOKEN_OR = 2,
	TOKEN_SEMICOLON = 3,

	TOKEN_PIPE = 4,

	TOKEN_WORD = 5,
	TOKEN_REDIR_IN = 6,
	TOKEN_REDIR_OUT	= 7,
	TOKEN_REDIR_APPEND	= 8,
	TOKEN_HEREDOC	= 9,

	TOKEN_NEWLINE = 10,
	TOKEN_LEFT_PAREN = 11,
	TOKEN_RIGHT_PAREN = 12,

	TOKEN_END = 13
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;


#endif