/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:52:42 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/17 20:37:23 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include "envp_copy.h"

# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '\"'
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

	TOKEN_END = 13,
	TOKEN_WILDCARD = 14
}					t_token_type;

typedef enum	s_quotes_status
{
	NO_QUOTES = 1,
	SINGLE_Q = 2,
	DOUBLE_Q = 3,
	MIXED = 4,
}	e_quotes_status;

typedef struct	s_piece
{
	char			*text;
	e_quotes_status	quotes;
	int				has_wild;
	int				has_env_v;
	int				has_tilde;
	struct	s_piece	*next;
}	t_piece;

typedef struct s_token
{
	t_token_type	type;
	e_quotes_status	stat;
	char			*value;
	t_piece			*pieces;
	t_piece			*pieces_tail;
	int				has_env_v;
	int				has_wild;
	struct s_token	*prev;
	struct s_token	*next;
	void 			(*free)(struct s_token *token);
}					t_token;



char				*get_cmd_path(char *cmd, char **envp);
t_token				*lexicalization(char *line);
t_token				*read_and_lexicalize();
char				**wildcard_expand(t_piece *piece);
char 				**expand_and_split_token(t_token *token, t_env *env, int last_exit_status);

int					validate_tokens(t_token *start);
void				free_tokens(t_token *tokens);

#endif
