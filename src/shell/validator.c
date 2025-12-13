#include "parsing.h"
#include "libft.h"
#include "error.h"
#include "constants.h"

static int	is_operator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PIPE
		|| token->type == TOKEN_OR
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_SEMICOLON);
}

static int	is_redir(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_APPEND
		|| token->type == TOKEN_HEREDOC);
}

static const char	*get_token_str(t_token *token)
{
	if (!token)
		return ("newline");
	if (token->type == TOKEN_PIPE)
		return ("|");
	if (token->type == TOKEN_OR)
		return ("||");
	if (token->type == TOKEN_AND)
		return ("&&");
	if (token->type == TOKEN_SEMICOLON)
		return (";");
	if (token->type == TOKEN_LEFT_PAREN)
		return ("(");
	if (token->type == TOKEN_RIGHT_PAREN)
		return (")");
	if (token->type == TOKEN_REDIR_IN)
		return ("<");
	if (token->type == TOKEN_REDIR_OUT)
		return (">");
	if (token->type == TOKEN_REDIR_APPEND)
		return (">>");
	if (token->type == TOKEN_HEREDOC)
		return ("<<");
	return ("newline");
}

static int	syntax_error_token(t_token *token)
{
	char *msg;
	msg = ft_strdup(SYNTAX_ERROR_UNEXPECTED_TOKEN);
	ft_strappend(&msg, (char *)get_token_str(token));
	ft_strappend(&msg, "'");
	output_error(NULL, msg);
	free(msg);
	// ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	// ft_putstr_fd((char *)get_token_str(token), 2);
	// ft_putstr_fd("'\n", 2);
	return (0);
}

static int	syntax_error_eof(void)
{
	// ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
	output_error(SYNTAX_ERROR, SYNTAX_ERROR_UNEXPECTED_EOF);
	return (0);
}

static int	check_paren_balance(t_token *curr, int paren_depth)
{
	if (curr->type == TOKEN_LEFT_PAREN)
		return (paren_depth + 1);
	if (curr->type == TOKEN_RIGHT_PAREN)
	{
		if (paren_depth <= 0)
			return (-1);
		return (paren_depth - 1);
	}
	return (paren_depth);
}

static int	check_empty_parens(t_token *curr)
{
	if (curr->type == TOKEN_LEFT_PAREN && curr->next
		&& curr->next->type == TOKEN_RIGHT_PAREN)
		return (syntax_error_token(curr->next));
	else
		return (1);
}

static int	check_redir_target(t_token *curr)
{
	if (is_redir(curr))
	{
		if (!curr->next || curr->next->type == TOKEN_END
			|| is_operator(curr->next) || is_redir(curr->next)
			|| curr->next->type == TOKEN_LEFT_PAREN
			|| curr->next->type == TOKEN_RIGHT_PAREN)
		{
			if (curr->next && curr->next->type != TOKEN_END)
				return (syntax_error_token(curr->next));
			else
				return (syntax_error_token(NULL));
		}
	}
	return (1);
}

static int	check_operators(t_token *curr, t_token *prev)
{
	if (!prev && is_operator(curr))
	{
		return (syntax_error_token(curr));
	}
	if (prev && is_operator(prev) && is_operator(curr))
	{
		return (syntax_error_token(curr));
	}
	return (1);
}

static int	check_paren_placement(t_token *curr, t_token *prev)
{
	if (curr->type == TOKEN_LEFT_PAREN && prev
		&& prev->type == TOKEN_WORD)
	{
		return (syntax_error_token(curr));
	}
	return (1);
}

int	validate_tokens(t_token *start)
{
	t_token	*curr;
	t_token	*prev;
	int		paren_depth;

	if (!start || start->type == TOKEN_END)
		return (1);
	curr = start;
	prev = NULL;
	paren_depth = 0;
	while (curr && curr->type != TOKEN_END)
	{
		paren_depth = check_paren_balance(curr, paren_depth);
		if (paren_depth < 0)
			return (syntax_error_token(curr));
		if (!check_empty_parens(curr))
			return (0);
		if (!check_redir_target(curr))
			return (0);
		if (!check_operators(curr, prev))
			return (0);
		if (!check_paren_placement(curr, prev))
			return (0);
		prev = curr;
		curr = curr->next;
	}
	if (paren_depth > 0)
		return (syntax_error_eof());
	if (prev && is_operator(prev) && prev->type != TOKEN_SEMICOLON)
		return (syntax_error_eof());
	return (1);
}
