/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:10:57 by ybutkov           #+#    #+#             */
/*   Updated: 2025/08/02 19:10:05 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	fill_number(char *number, unsigned long n)
{
	int	len;

	len = 0;
	if (n == 0)
		number[len++] = '0';
	while (n)
	{
		number[len++] = n % 10 + '0';
		n /= 10;
	}
	return (len);
}

char	*ft_ultoa(unsigned long n)
{
	int		len;
	char	number[11];
	char	*res;
	char	*str;

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
