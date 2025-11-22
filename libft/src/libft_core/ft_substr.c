/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:34 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/13 12:59:15 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			s_len;
	size_t			res_len;
	char			*res;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	else if (s_len - start < len)
		res_len = s_len - start + 1;
	else
		res_len = len + 1;
	res = malloc(res_len * sizeof(char));
	if (res == NULL)
		return (NULL);
	res[0] = '\0';
	if (!(start > s_len - 1))
		ft_strlcpy(res, &s[start], res_len);
	return (res);
}
