/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:13:00 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/13 12:49:38 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dst, const char *src)
{
	char	*res;

	res = dst;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (res);
}

char	*ft_strdup(const char *s1)
{
	void	*res;

	res = malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	return (ft_strcpy(res, s1));
}
