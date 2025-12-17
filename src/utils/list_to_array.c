/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:00:00 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/16 23:14:35 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include <stdlib.h>

void	**list_to_array_generic(t_list *lst, void *(*convert)(void *))
{
	void	**arr;
	int		size;
	int		i;

	size = ft_lstsize(lst);
	arr = (void **)malloc(sizeof(void *) * (size + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (lst)
	{
		if (convert)
			arr[i] = convert(lst->content);
		else
			arr[i] = lst->content;
		lst = lst->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**list_to_array(t_list *lst)
{
	return ((char **)list_to_array_generic(lst, NULL));
}

void	empty_func(void *p)
{
	(void)p;
}
