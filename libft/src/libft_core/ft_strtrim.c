/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:22:34 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/10 10:28:56 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in_set(char ch, char const *set)
{
	while (*set)
	{
		if (ch == *set)
			return (1);
		set = set + 1;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return (ft_strdup(s1));
	start = 0;
	end = ft_strlen(s1) - 1;
	while (in_set(s1[start], set))
		start++;
	if (start > end)
		return (ft_strdup(""));
	while (in_set(s1[end], set))
		end--;
	return (ft_substr(s1, start, end - start + 1));
}
