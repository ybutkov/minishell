/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_base_basic.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:11:32 by ybutkov           #+#    #+#             */
/*   Updated: 2025/09/09 12:11:34 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stack_base.h"

void	*stack_head(t_stack *s)
{
	if (!s->head)
		return (NULL);
	return (s->head->data);
}

void	*stack_tail(t_stack *s)
{
	if (!s->tail)
		return (NULL);
	return (s->tail->data);
}

void	*stack_peek(t_stack *s)
{
	if (!s->head)
		return (NULL);
	return (s->head->data);
}
