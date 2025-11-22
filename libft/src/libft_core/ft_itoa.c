/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:43:38 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/09 12:33:26 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	fill_number(char *number, int n)
{
	unsigned int	un;
	int				len;

	len = 0;
	if (n == 0)
	{
		number[len++] = '0';
		un = 0;
	}
	else if (n < 0)
		un = -n;
	else
		un = n;
	while (un)
	{
		number[len++] = un % 10 + '0';
		un /= 10;
	}
	if (n < 0)
		number[len++] = '-';
	return (len);
}

char	*ft_itoa(int n)
{
	int				len;
	char			number[11];
	char			*res;
	char			*str;

	len = fill_number(number, n);
	res = malloc((len + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	str = res;
	while (len--)
		*res++ = number[len];
	*res = '\0';
	return (str);
}
