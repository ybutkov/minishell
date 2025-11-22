/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_fixtures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:24:35 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/22 19:59:35 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsing.h"

static char *dupstr(const char *s)
{
	if (!s)
		return (NULL);
	// return (strdup(s));
	return (ft_strdup(s));
}

static t_token *set_prev_links(t_token *head)
{
	t_token *cur;
	t_token *prev;

	cur = head;
	prev = NULL;
	while (cur)
	{
		cur->prev = prev;
		prev = cur;
		cur = cur->next;
	}
	return (head);
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	token->prev = NULL;
	token->free = free_token;
	return (token);
}

// echo "Hello World" | grep Hello > out.txt
t_token *tokens_echo_grep_redirect(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("Hello World"));
	t_token *t3 = create_token(TOKEN_PIPE, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t5 = create_token(TOKEN_WORD, dupstr("Hello"));
	t_token *t6 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t7 = create_token(TOKEN_WORD, dupstr("out.txt"));
	t_token *t8 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	return (set_prev_links(t1));
}

// cat <<EOF | sed 's/foo/bar/g' >> results.txt

t_token *tokens_heredoc_sed_append(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t2 = create_token(TOKEN_HEREDOC, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("EOF"));
	t_token *t4 = create_token(TOKEN_PIPE, NULL);
	t_token *t5 = create_token(TOKEN_WORD, dupstr("sed"));
	t_token *t6 = create_token(TOKEN_WORD, dupstr("s/foo/bar/g"));
	t_token *t7 = create_token(TOKEN_REDIR_APPEND, NULL);
	t_token *t8 = create_token(TOKEN_WORD, dupstr("results.txt"));
	t_token *t9 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	return (set_prev_links(t1));
}

// export PATH="$PATH:~/.local/bin" && echo $PATH || echo "export failed"
t_token *tokens_export_and_echo_or(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("export"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("PATH=$PATH:~/.local/bin"));
	t_token *t3 = create_token(TOKEN_AND, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t5 = create_token(TOKEN_WORD, dupstr("$PATH"));
	t_token *t6 = create_token(TOKEN_OR, NULL);
	t_token *t7 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t8 = create_token(TOKEN_WORD, dupstr("export failed"));
	t_token *t9 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	return (set_prev_links(t1));
}

// (cd /tmp && ls -la) | wc -l
t_token *tokens_group_and_pipe(void)
{
	t_token *t1 = create_token(TOKEN_LEFT_PAREN, NULL);
	t_token *t2 = create_token(TOKEN_WORD, dupstr("cd"));
	t_token *t3 = create_token(TOKEN_WORD, dupstr("/tmp"));
	t_token *t4 = create_token(TOKEN_AND, NULL);
	t_token *t5 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t6 = create_token(TOKEN_WORD, dupstr("-la"));
	t_token *t7 = create_token(TOKEN_RIGHT_PAREN, NULL);
	t_token *t8 = create_token(TOKEN_PIPE, NULL);
	t_token *t9 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t10 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t11 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	t10->next = t11;
	return (set_prev_links(t1));
}

// echo 'single quotes'"and double" ; printf "%s\n" "done" >> log.txt
t_token *tokens_quotes_semi_append(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("single quotesand double"));
	t_token *t3 = create_token(TOKEN_SEMICOLON, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("printf"));
	t_token *t5 = create_token(TOKEN_WORD, dupstr("%s\\n"));
	t_token *t6 = create_token(TOKEN_WORD, dupstr("done"));
	t_token *t7 = create_token(TOKEN_REDIR_APPEND, NULL);
	t_token *t8 = create_token(TOKEN_WORD, dupstr("log.txt"));
	t_token *t9 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	return (set_prev_links(t1));
}

// 6) ls -la
t_token *tokens_simple_command(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("-la"));
	t_token *t3 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	return (set_prev_links(t1));
}

// 7) cat < input.txt
t_token *tokens_input_redirect(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t2 = create_token(TOKEN_REDIR_IN, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("input.txt"));
	t_token *t4 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	return (set_prev_links(t1));
}

// ls | wc -l
t_token *tokens_simple_pipe(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_PIPE, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t4 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t5 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	return (set_prev_links(t1));
}

// echo hello && echo world
t_token *tokens_and_commands(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("hello"));
	t_token *t3 = create_token(TOKEN_AND, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t5 = create_token(TOKEN_WORD, dupstr("world"));
	t_token *t6 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	return (set_prev_links(t1));
}

// echo test || echo fail
t_token *tokens_or_commands(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("test"));
	t_token *t3 = create_token(TOKEN_OR, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t5 = create_token(TOKEN_WORD, dupstr("fail"));
	t_token *t6 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	return (set_prev_links(t1));
}

// ls ; pwd ; echo done
t_token *tokens_multiple_semicolon(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_SEMICOLON, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("pwd"));
	t_token *t4 = create_token(TOKEN_SEMICOLON, NULL);
	t_token *t5 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t6 = create_token(TOKEN_WORD, dupstr("done"));
	t_token *t7 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	return (set_prev_links(t1));
}

// cat NEW_TOKEN_FIXTURES.md | grep pattern > output.txt
t_token *tokens_pipe_with_redirects(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("NEW_TOKEN_FIXTURES.md"));
	t_token *t3 = create_token(TOKEN_PIPE, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t5 = create_token(TOKEN_WORD, dupstr("WORD"));
	t_token *t6 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t7 = create_token(TOKEN_WORD, dupstr("output.txt"));
	t_token *t8 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	return (set_prev_links(t1));
}

// echo test > out.txt && cat out.txt
t_token *tokens_redirect_and_command(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("echo"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("test"));
	t_token *t3 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("out.txt"));
	t_token *t5 = create_token(TOKEN_AND, NULL);
	t_token *t6 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t7 = create_token(TOKEN_WORD, dupstr("out.txt"));
	t_token *t8 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	return (set_prev_links(t1));
}

// 14) ls | grep i | wc -l
t_token *tokens_triple_pipe(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_PIPE, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t4 = create_token(TOKEN_WORD, dupstr("i"));
	t_token *t5 = create_token(TOKEN_PIPE, NULL);
	t_token *t6 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t7 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t8 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	return (set_prev_links(t1));
}

// 14) ls | grep i | wc -tt
t_token *tokens_triple_pipe_error(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_PIPE, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t4 = create_token(TOKEN_WORD, dupstr("i"));
	t_token *t5 = create_token(TOKEN_PIPE, NULL);
	t_token *t6 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t7 = create_token(TOKEN_WORD, dupstr("-tt"));
	t_token *t8 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	return (set_prev_links(t1));
}

// 15) cat minishell.c | grep int | sort | uniq | wc -l
t_token *tokens_five_pipe_chain(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("minishell.c"));
	t_token *t3 = create_token(TOKEN_PIPE, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t5 = create_token(TOKEN_WORD, dupstr("int"));
	t_token *t6 = create_token(TOKEN_PIPE, NULL);
	t_token *t7 = create_token(TOKEN_WORD, dupstr("sort"));
	t_token *t8 = create_token(TOKEN_PIPE, NULL);
	t_token *t9 = create_token(TOKEN_WORD, dupstr("uniq"));
	t_token *t10 = create_token(TOKEN_PIPE, NULL);
	t_token *t11 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t12 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t13 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	t10->next = t11;
	t11->next = t12;
	t12->next = t13;
	return (set_prev_links(t1));
}

// 16) cat < minishell_test.log | grep home | wc -l
t_token *tokens_input_pipe_chain(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t2 = create_token(TOKEN_REDIR_IN, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("minishell_test.log"));
	t_token *t4 = create_token(TOKEN_PIPE, NULL);
	t_token *t5 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t6 = create_token(TOKEN_WORD, dupstr("home"));
	t_token *t7 = create_token(TOKEN_PIPE, NULL);
	t_token *t8 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t9 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t10 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	return (set_prev_links(t1));
}

// 16a) < minishell_test.log cat  | grep home | wc -l
t_token *tokens_input_pipe_chain_1_redir(void)
{
	t_token *t1 = create_token(TOKEN_REDIR_IN, NULL);
	t_token *t2 = create_token(TOKEN_WORD, dupstr("minishell_test.log"));
	t_token *t3 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t4 = create_token(TOKEN_PIPE, NULL);
	t_token *t5 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t6 = create_token(TOKEN_WORD, dupstr("home"));
	t_token *t7 = create_token(TOKEN_PIPE, NULL);
	t_token *t8 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t9 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t10 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	return (set_prev_links(t1));
}

// 17) ls -l | grep txt > files.txt
t_token *tokens_pipe_output_redirect(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t3 = create_token(TOKEN_PIPE, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t5 = create_token(TOKEN_WORD, dupstr("txt"));
	t_token *t6 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t7 = create_token(TOKEN_WORD, dupstr("files.txt"));
	t_token *t8 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	return (set_prev_links(t1));
}

// 18) cat file1.txt file2.txt | sort | uniq >> output.txt
t_token *tokens_pipe_append_redirect(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("file1.txt"));
	t_token *t3 = create_token(TOKEN_WORD, dupstr("file2.txt"));
	t_token *t4 = create_token(TOKEN_PIPE, NULL);
	t_token *t5 = create_token(TOKEN_WORD, dupstr("sort"));
	t_token *t6 = create_token(TOKEN_PIPE, NULL);
	t_token *t7 = create_token(TOKEN_WORD, dupstr("uniq"));
	t_token *t8 = create_token(TOKEN_REDIR_APPEND, NULL);
	t_token *t9 = create_token(TOKEN_WORD, dupstr("output.txt"));
	t_token *t10 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	return (set_prev_links(t1));
}

// 19) cat < in.txt | tr 'a-z' 'A-Z' > out.txt
t_token *tokens_input_pipe_output(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t2 = create_token(TOKEN_REDIR_IN, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("in.txt"));
	t_token *t4 = create_token(TOKEN_PIPE, NULL);
	t_token *t5 = create_token(TOKEN_WORD, dupstr("tr"));
	t_token *t6 = create_token(TOKEN_WORD, dupstr("a-z"));
	t_token *t7 = create_token(TOKEN_WORD, dupstr("A-Z"));
	t_token *t8 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t9 = create_token(TOKEN_WORD, dupstr("out.txt"));
	t_token *t10 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	return (set_prev_links(t1));
}

// 20) ls | grep test | wc -l > count.txt | cat
t_token *tokens_multi_pipe_multi_redirect(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_PIPE, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t4 = create_token(TOKEN_WORD, dupstr("test"));
	t_token *t5 = create_token(TOKEN_PIPE, NULL);
	t_token *t6 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t7 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t8 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t9 = create_token(TOKEN_WORD, dupstr("count.txt"));
	t_token *t10 = create_token(TOKEN_PIPE, NULL);
	t_token *t11 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t12 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	t10->next = t11;
	t11->next = t12;
	return (set_prev_links(t1));
}

// 21) cat < NEW_TOKEN_FIXTURES.md > output_21.txt | wc -l
// Command with both input and output redirect, then piped
t_token *tokens_dual_redirect_pipe(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("cat"));
	t_token *t2 = create_token(TOKEN_REDIR_IN, NULL);
	t_token *t3 = create_token(TOKEN_WORD, dupstr("NEW_TOKEN_FIXTURES.md"));
	t_token *t4 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t5 = create_token(TOKEN_WORD, dupstr("output_21.txt"));
	t_token *t6 = create_token(TOKEN_PIPE, NULL);
	t_token *t7 = create_token(TOKEN_WORD, dupstr("wc"));
	t_token *t8 = create_token(TOKEN_WORD, dupstr("-l"));
	t_token *t9 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	return (set_prev_links(t1));
}

// 22) ls -la > out_22.txt < /dev/null | grep ell >> out_23.txt
// Multiple redirects on first command, pipe, then append on second
t_token *tokens_complex_multi_redirect(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("-la"));
	t_token *t3 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("out_22.txt"));
	t_token *t5 = create_token(TOKEN_REDIR_IN, NULL);
	t_token *t6 = create_token(TOKEN_WORD, dupstr("/dev/null"));
	t_token *t7 = create_token(TOKEN_PIPE, NULL);
	t_token *t8 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t9 = create_token(TOKEN_WORD, dupstr("ell"));
	t_token *t10 = create_token(TOKEN_REDIR_APPEND, NULL);
	t_token *t11 = create_token(TOKEN_WORD, dupstr("out_23.txt"));
	t_token *t12 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	t10->next = t11;
	t11->next = t12;
	return (set_prev_links(t1));
}

// ls -la > out_21.txt > out_22.txt | <out_22.txt grep ell >> out_23.txt
// Multiple redirects on first command, pipe, then append on second
t_token *tokens_complex_multi_redirect_2(void)
{
	t_token *t1 = create_token(TOKEN_WORD, dupstr("ls"));
	t_token *t2 = create_token(TOKEN_WORD, dupstr("-la"));
	t_token *t3 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t4 = create_token(TOKEN_WORD, dupstr("out_21.txt"));
	t_token *t5 = create_token(TOKEN_REDIR_OUT, NULL);
	t_token *t6 = create_token(TOKEN_WORD, dupstr("out_22.txt"));
	t_token *t7 = create_token(TOKEN_PIPE, NULL);
	t_token *t8 = create_token(TOKEN_REDIR_IN, NULL);
	t_token *t9 = create_token(TOKEN_WORD, dupstr("out_22.txt"));
	t_token *t10 = create_token(TOKEN_WORD, dupstr("grep"));
	t_token *t11 = create_token(TOKEN_WORD, dupstr("ell"));
	t_token *t12 = create_token(TOKEN_REDIR_APPEND, NULL);
	t_token *t13 = create_token(TOKEN_WORD, dupstr("out_23.txt"));
	t_token *t14 = create_token(TOKEN_END, NULL);

	t1->next = t2;
	t2->next = t3;
	t3->next = t4;
	t4->next = t5;
	t5->next = t6;
	t6->next = t7;
	t7->next = t8;
	t8->next = t9;
	t9->next = t10;
	t10->next = t11;
	t11->next = t12;
	t12->next = t13;
	t13->next = t14;
	return (set_prev_links(t1));
}
