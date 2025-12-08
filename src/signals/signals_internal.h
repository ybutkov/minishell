/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 13:06:52 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/08 13:09:18 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_INTERNAL_H
# define SIGNALS_INTERNAL_H

# define _POSIX_C_SOURCE 200809L //for some reason doesn't work without precise definition
// without it's just a c standard set
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

void	set_signals_parent_interactive(void);
void	set_signals_waiting_parent(void);
void	set_signals_child(void);
void	handle_sigint_parent(int sig);
void	handle_sigquit_parent(int sig);

#endif