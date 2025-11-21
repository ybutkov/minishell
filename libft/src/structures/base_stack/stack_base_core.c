/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_base_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:12:31 by ybutkov           #+#    #+#             */
/*   Updated: 2025/09/10 19:37:38 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stack_base_internal.h"

void	stack_free(t_stack *s)
{
	t_stack_node	*node;

	while (s->head)
	{
		node = s->head;
		s->head = node->next;
		free(node->data);
		free(node);
	}
	s->tail = NULL;
	free(s);
}

void	**stack_to_array(t_stack *stack)
{
	t_stack_node	*current;
	void			**result;
	size_t			i;

	if (!stack)
		return (NULL);
	result = malloc(stack->size * sizeof(void *));
	if (!result)
		return (NULL);
	i = 0;
	current = stack->head;
	while (current)
	{
		result[i++] = current->data;
		current = current->next;
	}
	return (result);
}

t_stack	*stack_create(void)
{
	t_stack	*s;

	s = malloc(sizeof(t_stack));
	if (!s)
		return (NULL);
	s->head = NULL;
	s->tail = NULL;
	s->size = 0;
	s->push = stack_push;
	s->pop = stack_pop;
	s->peek = stack_peek;
	s->free = stack_free;
	s->head_data = stack_head;
	s->tail_data = stack_tail;
	s->swap = stack_swap;
	s->reverse_rotate = stack_reverse_rotate;
	s->rotate = stack_rotate;
	s->to_array = stack_to_array;
	return (s);
}
