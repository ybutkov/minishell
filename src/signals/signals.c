/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:31:47 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 16:46:38 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ~~~Interactive (TTY):
// Ctrl+C (SIGINT): do not exit; 
//print newline; redisplay prompt; clear current input.
// Ctrl+\ (SIGQUIT): ignore (no “Quit” message).
// Ctrl+D: not a signal; 
//readline returns NULL → exit cleanly (usually return 0).
// ~~~During command execution (child processes):
// Ctrl+C: terminate foreground child; 
//parent sets exit status to 130; print newline.
// Ctrl+\: terminate foreground child; 
//print “Quit: 3”; parent sets exit status to 131.
// ~~~Non-interactive (piped input):
// Usually let default signal behavior apply to children; parent should
//not print prompts; if readline isn’t used 
//(you use get_next_line), handle EOF by exiting.

#include "signals_internal.h"
#include "signals.h"

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
