/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:16:20 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/13 12:56:10 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	int	i;

	if (*needle == '\0')
		return ((char *)haystack);
	i = 0;
	if (haystack == NULL && !len)
		return (NULL);
	while (*haystack && (len - i))
	{
		if (haystack[i] == needle[i])
			i++;
		else
			i = 0;
		if (needle[i] == '\0')
			return ((char *)(haystack));
		if (i == 0)
		{
			haystack++;
			len--;
		}
	}
	return (NULL);
}
