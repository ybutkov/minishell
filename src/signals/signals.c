/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:31:47 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/17 23:12:38 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ~~~Interactive (TTY):
// Ctrl+C (SIGINT): do not exit; print newline; redisplay prompt; clear current input.
// Ctrl+\ (SIGQUIT): ignore (no “Quit” message).
// Ctrl+D: not a signal; readline returns NULL → exit cleanly (usually return 0).
// ~~~During command execution (child processes):
// Ctrl+C: terminate foreground child; parent sets exit status to 130; print newline.
// Ctrl+\: terminate foreground child; print “Quit: 3”; parent sets exit status to 131.
// ~~~Non-interactive (piped input):
// Usually let default signal behavior apply to children; parent should
//not print prompts; if readline isn’t used (you use get_next_line), handle EOF by exiting.

#include "signals_internal.h"


void	set_signals_parent_interactive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_parent;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

//before each waitpid
//after waiting restore back to parent interactive
void	set_signals_waiting_parent(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

//basically just dropping to default behavior
//at the beginning of each child
void	set_signals_child(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_DFL;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	set_signals_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

//ctrl+c in interactive: print a newline, clear the current line, show a fresh prompt
void	handle_sigint_parent(int sig)
// sig - signal number that triggered the handler (2 in this case)
{
	(void)sig;
	write(1, "\n", 1); // signal handler must use async-signal-safe functions
	rl_on_new_line(); // updating internal cursor position
	rl_replace_line("", 0); //clearing the current line
	rl_redisplay(); // redrawing the prompt ($>) at screen
}

// //does nothing. another way - use SIG_IGN when defining
// void	handle_sigquit_parent(int sig)
// {
// 	(void)sig;
// }

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
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
