/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 18:27:18 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 14:21:21 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// getcwd(buffer, size) - a function, returning the current
//working directory of the process
#include "builtin_internal.h"
#include <unistd.h>

int	bi_pwd(void)
{
	char	buffer[BUFFER_PATH];
	int		len;

	if (getcwd(buffer, BUFFER_PATH) == NULL)
		return (1);
	else
	{
		len = ft_strlen(buffer);
		write(STDOUT_FILENO, buffer, len);
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
}
