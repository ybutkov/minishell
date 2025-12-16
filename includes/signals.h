/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 02:03:51 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/16 19:56:21 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define _POSIX_C_SOURCE 200809L
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

void	set_signals_parent_interactive(void);
void	set_signals_waiting_parent(void);
void	set_signals_child(void);
void	set_signals_heredoc(void);
void	disable_ctrl_echo(void);

#endif