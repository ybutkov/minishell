/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_builtin_type.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 03:31:36 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 04:36:30 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_internal.h"
#include "constants.h"
#include "error.h"
#include "libft.h"
#include "utils.h"
#include "builtin.h"
#include "shell_utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "signals.h"
#include <sys/types.h>
#include <dirent.h>

static char	**get_built_in_list(void)
{
	static char	*builtins[8];
	static int	initialized;

	if (!initialized)
	{
		builtins[0] = "echo";
		builtins[1] = "cd";
		builtins[2] = "pwd";
		builtins[3] = "export";
		builtins[4] = "unset";
		builtins[5] = "env";
		builtins[6] = "exit";
		builtins[7] = NULL;
		initialized = 1;
	}
	return (builtins);
}

int	builtin_func(char *command)
{
	char	**built_ins;
	int		i;

	if (!command)
		return (-1);
	built_ins = get_built_in_list();
	i = 0;
	while (built_ins[i])
	{
		if (ft_strcmp(command, built_ins[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}
