/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:21:56 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/09 12:33:46 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	value;
	unsigned char	*str;

	value = (unsigned char)c;
	str = (unsigned char *)s;
	while (n--)
	{
		if (*str == value)
			return (str);
		str++;
	}
	return (NULL);
}
