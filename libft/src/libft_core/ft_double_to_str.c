/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_double_to_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:09:39 by ybutkov           #+#    #+#             */
/*   Updated: 2025/09/30 17:00:46 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

static void	add_fractional_part(char *str, double abs_n, int precision,
		size_t len)
{
	int		i;
	int		nbr;
	long	fract_part;

	fract_part = (long)((abs_n - (long)abs_n) * powl(10, precision));
	str[len++] = '.';
	i = 0;
	while (++i <= precision)
	{
		nbr = (fract_part / (long)powl(10, precision - i)) % 10;
		str[len + i - 1] = '0' + nbr;
	}
	str[len + i - 1] = '\0';
}

char	*ft_double_to_str(double n, int precision)
{
	char	*str;
	char	*res;
	size_t	len;
	float	abs_n;

	abs_n = fabs(n);
	str = ft_lltoa((long)(abs_n));
	if (str == NULL)
		return (NULL);
	if (precision > 0)
	{
		len = ft_strlen(str);
		str = ft_realloc(str, len + 1, len + precision + 2);
		if (str == NULL)
			return (NULL);
		add_fractional_part(str, abs_n, precision, len);
	}
	if (n < 0)
	{
		res = ft_strjoin("-", str);
		free(str);
		return (res);
	}
	return (str);
}
