/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 11:26:58 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/23 22:36:30 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

// space, newline, tab - split tokens outside quotes;
// single quotes - everything inside literal;
// double quotes - everything literal, except for $, \, and `;
// \ - cancelling the meaning of the next sign, but not erasing it;
// # all the rest of the line is commented out;

t_token	*lexicalization(char *line)
{
	t_lex_inf	lex;

	lex_struct_init(line, &lex);
	while (line[lex.i])
	{
		if (is_whitespace(line[lex.i]))
			is_space(&lex);
		else if (line[lex.i] == '\'')
			is_single_quote(&lex);
		else if (line[lex.i] == '"')
			is_double_quote(&lex);
		else if (line[lex.i] == '#')
			break ;
		else if (line[0])
			is_space(&lex);
	}
	return (lex.head);
}

void lex_struct_init(char *line, t_lex_inf *lex)
{
	lex->line = line;
	lex->i = 0;
	lex->start = 0;
	lex->end = 0;
	lex->head = NULL;
	lex->tail = NULL;
	lex->error_code = 0;
}

// int	main(int argc, char **argv)
// {
//     char	*line = NULL;
//     size_t	len = 0;
//     t_token	*toks;
//     t_token *t;
//     t_piece *p;
//     int	i;

//     if (argc > 1)
//     {
//         /* join argv[1..] into a single test line */
//         size_t need = 1;
//         for (i = 1; i < argc; ++i)
//             need += strlen(argv[i]) + 1;
//         line = malloc(need);
//         if (!line) return (1);
//         line[0] = '\0';
//         for (i = 1; i < argc; ++i)
//         {
//             strcat(line, argv[i]);
//             if (i + 1 < argc) strcat(line, " ");
//         }
//     }
//     else
//     {
//         printf("Enter test line: ");
//         if (getline(&line, &len, stdin) == -1)
//             return (0);
//         /* strip newline */
//         if (line[0])
//         {
//             size_t l = strlen(line);
//             if (l && line[l - 1] == '\n')
//                 line[l - 1] = '\0';
//         }
//     }

//     toks = lexicalization(line);
//     printf("=== tokens ===\n");
//     i = 0;
//     for (t = toks; t; t = t->next)
//     {
//         printf("[%d] value: '%s' stat:%d\n", i++, t->value ? t->value : "(null)", (int)t->stat);
//         for (p = t->pieces; p; p = p->next)
//             printf("     piece (q=%d): '%s'\n", (int)p->quotes, p->full_str ? p->full_str : "(null)");
//     }
//     /* test run; not freeing all allocations here */
//     if (argc > 1)
//         free(line);
//     return (0);
// }