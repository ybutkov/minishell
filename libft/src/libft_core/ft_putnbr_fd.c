/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:10:03 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/13 12:46:29 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	un;
	int				len;
	char			number[12];

	len = 0;
	if (n == 0)
	{
		number[11 - len++] = '0';
		un = 0;
	}
	else if (n < 0)
		un = -n;
	else
		un = n;
	while (un)
	{
		number[11 - len++] = un % 10 + '0';
		un /= 10;
	}
	if (n < 0)
		number[11 - len++] = '-';
	write(fd, &number[12 - len], len);
}
