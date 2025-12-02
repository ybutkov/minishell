/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 18:27:18 by ashadrin          #+#    #+#             */
/*   Updated: 2025/11/30 18:59:40 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// getcwd(buffer, size) - a function, returning the current
//working directory of the process
#include "builtin_internal.h"

int	bi_pwd()
{
	char buffer[BUFFER_PATH];
	if (getcwd(buffer, BUFFER_PATH) == NULL)
		return (1);
	else
	{
		printf("%s\n", buffer);
		return (0);
	}
}