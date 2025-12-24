/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstinsert_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 03:32:30 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 03:47:19 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstinsert(t_list *after, t_list *new)
{
	if (!after || !new)
		return ;
	new->next = after->next;
	after->next = new;
}
