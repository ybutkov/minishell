/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strappend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 22:19:05 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/06 22:39:39 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strappend(char **to, char *append)
{
	char	*new_str;

	if (append == NULL)
		return (1);
	if (*to == NULL)
	{
		new_str = ft_strdup(append);
		if (new_str == NULL)
			return (0);
		*to = new_str;
		return (1);
	}
	new_str = ft_strjoin(*to, append);
	if (new_str == NULL)
		return (0);
	free(*to);
	*to = new_str;
	return (1);
}
