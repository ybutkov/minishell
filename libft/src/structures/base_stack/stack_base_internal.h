/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_base_internal.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:17:17 by ybutkov           #+#    #+#             */
/*   Updated: 2025/09/09 12:17:19 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_BASE_INTERNAL_H
# define STACK_BASE_INTERNAL_H

# include "stack_base.h"

void			*stack_head(t_stack *s);
void			*stack_tail(t_stack *s);
t_stack_node	*stack_push(t_stack *s, void *data);
void			*stack_pop(t_stack *s);
void			*stack_peek(t_stack *s);
void			stack_free(t_stack *s);
void			stack_swap(t_stack *stack);
void			stack_reverse_rotate(t_stack *stack);
void			stack_rotate(t_stack *stack);
void			**stack_to_array(t_stack *stack);

#endif
