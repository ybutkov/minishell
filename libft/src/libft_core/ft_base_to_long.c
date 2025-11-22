/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_base_to_long.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:42:53 by ybutkov           #+#    #+#             */
/*   Updated: 2025/09/30 17:00:23 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_index_in_base(char *base_from, char ch)
{
	int	index;

	index = 0;
	while (ch != base_from[index])
		index++;
	return (index);
}

static int	calc_number(char *nbr, char *base_from, int number)
{
	return (number * ft_strlen(base_from) + get_index_in_base(base_from, *nbr));
}

int	check_base(char *base)
{
	int	len_base;
	int	inner_i;

	len_base = 0;
	while (*base)
	{
		if (*base == '+' || *base == '-' || *base == ' '
			|| (*base >= 9 && *base <= 13))
			return (-1);
		inner_i = 1;
		while (base[inner_i])
			if (*base == base[inner_i++])
				return (-1);
		base++;
		len_base++;
	}
	if (len_base < 2)
		return (-1);
	return (len_base);
}

long	base_to_long(char *nbr, char *base_from)
{
	int	res;
	int	number;

	res = 1;
	number = 0;
	while (*nbr == ' ' || (*nbr >= 9 && *nbr <= 13))
		nbr = nbr + 1;
	while (*nbr)
	{
		if (*nbr == '-')
			res = -1 * res;
		if (*nbr == '+' || *nbr == '-')
		{
			nbr = nbr + 1;
			continue ;
		}
		if (ft_strchr(base_from, *nbr))
		{
			number = calc_number(nbr, base_from, number);
			nbr = nbr + 1;
			continue ;
		}
		break ;
	}
	return (res * number);
}
