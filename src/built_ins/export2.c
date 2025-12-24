/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:31:52 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 16:33:55 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include "error.h"

void	output_declare(t_env *env)
{
	t_env_pair	*current;
	int			len;

	current = env->head;
	while (current != NULL)
	{
		if (current->key)
		{
			write(STDOUT_FILENO, MSG_DECLARE_X, ft_strlen(MSG_DECLARE_X));
			len = ft_strlen(current->key);
			write(STDOUT_FILENO, current->key, len);
			if (current->value)
			{
				write(STDOUT_FILENO, "=", 1);
				len = ft_strlen(current->value);
				write(STDOUT_FILENO, current->value, len);
			}
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
}

int	check_flags(t_env *env, char **args, int *i)
{
	char	*err_msg;

	*i = 1;
	if (args[1] && (args[1][0] == '-'))
	{
		if (ft_strcmp(args[1], "-p") == 0)
		{
			if (args[2] == NULL)
				output_declare(env);
			*i = 2;
			return (OK);
		}
		err_msg = ft_strdup(MSG_EXPORT_2_COLON);
		if (args[1][1] == 'p')
			err_msg[ft_strlen(MSG_EXPORT_2_COLON) - 1] = args[1][2];
		else
			err_msg[ft_strlen(MSG_EXPORT_2_COLON) - 1] = args[1][1];
		output_error(err_msg, MSG_INVALID_OPTION);
		free(err_msg);
		return (ERROR);
	}
	return (OK);
}

void	safe_error_output(int *status, char *arg)
{
	char			*obj;

	*status = 1;
	obj = ft_strdup("export: ");
	if (!obj || ft_strappend(&obj, arg) != 1)
		output_error(MALLOC_ERROR, NULL);
	else
		output_error(obj, NOT_VALID_IDENTIFIER);
	if (obj)
		free(obj);
}

void	handle_plus_equal(t_env *env, t_export_par *var,
	char *arg, int *is_valid)
{
	char	*str;

	split_key(arg, var, "=");
	*is_valid = is_valid_key(var->key);
	if (*is_valid == 1)
		env->set_pair(env, var->key, var->value);
	else
	{
		split_key(arg, var, "+=");
		*is_valid = is_valid_key(var->key);
		if (*is_valid == 1)
		{
			str = env->get_value(env, var->key);
			if (str)
				str = ft_strdup(env->get_value(env, var->key));
			ft_strappend(&str, var->value);
			env->set_pair(env, var->key, str);
		}
	}
}

void	key_value_free(t_export_par *var)
{
	if (var->key)
		free(var->key);
	if (var->value)
		free(var->value);
}
