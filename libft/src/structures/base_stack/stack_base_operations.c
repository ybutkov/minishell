/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_base_operations.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:13:54 by ybutkov           #+#    #+#             */
/*   Updated: 2025/09/09 12:14:04 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stack_base.h"

t_stack_node	*stack_push(t_stack *s, void *data)
{
	t_stack_node	*node;

	node = malloc(sizeof(t_stack_node));
	if (!node)
		return (NULL);
	node->data = data;
	node->next = s->head;
	s->head = node;
	s->size++;
	if (s->tail == NULL)
	{
		s->tail = node;
	}
	return (node);
}

void	*stack_pop(t_stack *s)
{
	t_stack_node	*node;
	void			*data;

	if (!s->head)
		return (NULL);
	node = s->head;
	data = node->data;
	s->head = node->next;
	if (s->head == NULL)
	{
		s->tail = NULL;
	}
	free(node);
	s->size--;
	return (data);
}

void	stack_swap(t_stack *stack)
{
	t_stack_node	*first;
	t_stack_node	*second;

	if (stack == NULL || stack->size < 2)
		return ;
	first = stack->head;
	second = first->next;
	if (second == NULL)
		return ;
	first->next = second->next;
	second->next = first;
	stack->head = second;
	if (first->next == NULL)
		stack->tail = first;
}

void	stack_reverse_rotate(t_stack *stack)
{
	t_stack_node	*last;
	t_stack_node	*second_last;

	if (stack == NULL || stack->size < 2)
		return ;
	last = stack->tail;
	second_last = stack->head;
	while (second_last->next && second_last->next != last)
		second_last = second_last->next;
	if (second_last == last)
		return ;
	second_last->next = NULL;
	last->next = stack->head;
	stack->head = last;
	stack->tail = second_last;
}

void	stack_rotate(t_stack *stack)
{
	t_stack_node	*first;
	t_stack_node	*new_head;

	if (stack == NULL || stack->size < 2)
		return ;
	first = stack->head;
	new_head = first->next;
	if (new_head == NULL)
		return ;
	stack->head = new_head;
	first->next = NULL;
	stack->tail->next = first;
	stack->tail = first;
}
