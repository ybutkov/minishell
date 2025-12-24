/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_and_disable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:45:12 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 16:47:16 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals_internal.h"
#include "signals.h"

volatile sig_atomic_t	g_heredoc_interrupted = 0;

//ctrl+c in interactive: print a newline, 
//clear the current line, show a fresh prompt
void	handle_sigint_parent(int sig)
// sig - signal number that triggered the handler (2 in this case)
{
	(void)sig;
	if (!g_heredoc_interrupted)
		write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// //does nothing. another way - use SIG_IGN when defining
// void	handle_sigquit_parent(int sig)
// {
// 	(void)sig;
// }

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	disable_ctrl_echo(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
