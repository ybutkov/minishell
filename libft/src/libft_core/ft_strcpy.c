/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:15:52 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 16:17:09 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	char	*res;

	if (dst == NULL || src == NULL)
		return (dst);
	res = dst;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (res);
}
