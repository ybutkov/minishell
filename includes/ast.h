/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:56 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/07 15:42:00 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "pipex.h"
# include <stdlib.h>

typedef struct s_ast_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	void				*content;

	struct s_ast_node	*(*get_left)(struct s_ast_node *node);
	struct s_ast_node	*(*get_right)(struct s_ast_node *node);
	void				(*set_left)(struct s_ast_node *node,
			struct s_ast_node *left);
	void				(*set_right)(struct s_ast_node *node,
			struct s_ast_node *right);
	void				*(*get_content)(struct s_ast_node *node);
	void				*(*set_content)(struct s_ast_node *node, void *content);

}						t_ast_node;

typedef struct s_ast
{
	t_ast_node			*root;

	void				(*free)(struct s_ast *ast,
			void (*free_content)(void *));
	void				(*set_root)(struct s_ast *ast, t_ast_node *root);
	t_ast_node			*(*get_root)(struct s_ast *ast);
}						t_ast;

// Function declarations
t_ast					*create_ast_tree(void);
t_ast_node				*create_ast_node(void *content);

#endif
