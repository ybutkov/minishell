/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:56:09 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/09 17:08:49 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in_set(char ch, char *set)
{
	while (*set)
	{
		if (ch == *set)
			return (1);
		set = set + 1;
	}
	return (0);
}

static int	is_space(char ch)
{
	return (in_set(ch, " \x9\xA\xB\xC\xD"));
}

int	ft_atoi(const char *str)
{
	int	sign;
	int	res;

	while (is_space(*str))
		str++;
	sign = 1;
	res = 0;
	if (in_set(*str, "+-"))
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (in_set(*str, "0123456789"))
	{
		res = res * 10 + *str - '0';
		str++;
	}
	return (sign * res);
}
