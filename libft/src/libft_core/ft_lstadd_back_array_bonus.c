/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_array_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 18:41:18 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 18:44:38 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_nodes(t_list *start)
{
	t_list	*tmp;

	while (start)
	{
		tmp = start->next;
		free(start);
		start = tmp;
	}
}

static t_list	*build_chain(void **arr)
{
	t_list	*first;
	t_list	*last;
	t_list	*node;
	size_t	i;

	if (arr == NULL)
		return (NULL);
	first = NULL;
	last = NULL;
	i = 0;
	while (arr[i])
	{
		node = ft_lstnew(arr[i]);
		if (node == NULL)
			return (free_nodes(first), NULL);
		if (first == NULL)
			first = node;
		else
			last->next = node;
		last = node;
		i++;
	}
	return (first);
}

int	ft_lstadd_back_array(t_list **lst, void **arr)
{
	t_list	*chain;

	if (lst == NULL || arr == NULL)
		return (1);
	if (arr[0] == NULL)
		return (1);
	chain = build_chain(arr);
	if (chain == NULL)
		return (0);
	if (*lst == NULL)
		*lst = chain;
	else
		ft_lstlast(*lst)->next = chain;
	return (1);
}
