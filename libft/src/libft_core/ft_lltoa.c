/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lltoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:17:08 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/16 19:32:20 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	fill_number(char *number, long long n)
{
	unsigned long long	un;
	int					len;

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

char	*ft_lltoa(long long n)
{
	int					len;
	char				number[21];
	char				*res;
	char				*str;

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
