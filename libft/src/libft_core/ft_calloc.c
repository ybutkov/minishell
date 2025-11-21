/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:01:45 by ybutkov           #+#    #+#             */
/*   Updated: 2025/08/02 19:06:36 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*res;

	if (size && (size * count) / size != count)
		return (NULL);
	res = malloc(count * size);
	if (res == NULL)
		return (NULL);
	ft_memset(res, 0, count * size);
	return (res);
}
