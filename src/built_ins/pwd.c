/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 18:27:18 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/15 21:51:22 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// getcwd(buffer, size) - a function, returning the current
//working directory of the process
#include "builtin_internal.h"
#include <unistd.h>

int	bi_pwd()
{
	char	buffer[BUFFER_PATH];
	int		len;

	if (getcwd(buffer, BUFFER_PATH) == NULL)
		return (1);
	else
	{
		// printf("%s\n", buffer);
		len = ft_strlen(buffer);
		write(STDOUT_FILENO, buffer, len);
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
}
