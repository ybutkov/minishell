/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_base.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:17:30 by ybutkov           #+#    #+#             */
/*   Updated: 2025/09/09 12:17:31 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_BASE_H
# define STACK_BASE_H

# include <stdlib.h>

typedef struct stack_node
{
	void				*data;
	struct stack_node	*next;
}						t_stack_node;

typedef struct s_stack
{
	t_stack_node		*head;
	t_stack_node		*tail;
	size_t				size;
	t_stack_node		*(*push)(struct s_stack *s, void *data);
	void				*(*pop)(struct s_stack *s);
	void				*(*peek)(struct s_stack *s);
	void				(*free)(struct s_stack *s);
	void				*(*head_data)(struct s_stack *s);
	void				*(*tail_data)(struct s_stack *s);
	void				(*swap)(struct s_stack *s);
	void				(*reverse_rotate)(struct s_stack *s);
	void				(*rotate)(struct s_stack *s);
	void				**(*to_array)(struct s_stack *s);
}						t_stack;

t_stack					*stack_create(void);

#endif
