/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:47:16 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 03:26:17 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "constants.h"
# include "envp_copy.h"
# include "libft.h"
# include "shell.h"
# include "shell_node.h"
# include <stdio.h>
# include <unistd.h>

typedef struct s_export_par
{
	char	*key;
	char	*value;
}			t_export_par;

int			builtin(int bi_func, t_cmd *cmd, t_shell *shell, int in_out[2]);

#endif
