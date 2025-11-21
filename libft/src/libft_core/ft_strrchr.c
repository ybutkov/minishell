/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:53:16 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/09 12:35:38 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	value;
	char	*str;
	char	*result;

	value = (char)c;
	str = (char *)s;
	result = NULL;
	while (*str)
	{
		if (*str == value)
			result = str;
		str += 1;
	}
	if (result)
		return (result);
	if (value == '\0')
		return (str);
	return (NULL);
}
