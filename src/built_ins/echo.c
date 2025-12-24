/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 16:26:58 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 14:21:46 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// recieves something like: args = ["echo", "-n", "hello", "world", NULL]

// with option -n

#include "builtin_internal.h"

static int	only_ns(char *string)
{
	int	i;

	i = 1;
	while (string[i])
	{
		if (string[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	bi_echo(char **args)
{
	int	i;
	int	flag;
	int	len;

	flag = 0;
	i = 1;
	while (args[i] != NULL)
	{
		if (ft_strncmp(args[i], "-n", 2) == 0 && only_ns(args[i]))
			flag = 1;
		else
		{
			len = ft_strlen(args[i]);
			write(1, args[i], len);
			if (args[i + 1] != NULL)
				write(1, " ", 1);
		}
		i++;
	}
	if (flag == 0)
		write(1, "\n", 1);
	return (0);
}

// int main()
// {
// 	char *args[] = { "echo", "-nnn", "please", "find", "me", NULL };
// 	int i = bi_echo(args);
// 	return (0);
// }
