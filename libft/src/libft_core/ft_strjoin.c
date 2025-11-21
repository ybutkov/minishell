/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:03:16 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/09 12:42:42 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	char	*new_str;
	int		res_len;

	if (s1 == NULL)
		return (NULL);
	if (s2 == NULL)
		return (ft_strdup(s1));
	res_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = malloc(res_len * sizeof(char));
	if (res == NULL)
		return (NULL);
	new_str = res;
	while (*s1)
		*new_str++ = *s1++;
	while (*s2)
		*new_str++ = *s2++;
	*new_str = '\0';
	return (res);
}
