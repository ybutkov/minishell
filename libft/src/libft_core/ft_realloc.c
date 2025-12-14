/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 13:28:04 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/14 22:42:54 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	min_size;
	char	*src;
	char	*dst;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);
	if (ptr == NULL)
		return (new_ptr);
	ft_memset(new_ptr, 0, new_size);
	if (old_size < new_size)
		min_size = old_size;
	else
		min_size = new_size;
	src = (char *)ptr;
	dst = (char *)new_ptr;
	ft_memmove(dst, src, min_size);
	free(ptr);
	return (new_ptr);
}
