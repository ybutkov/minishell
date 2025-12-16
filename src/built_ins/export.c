/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:35:12 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/16 03:26:34 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//export without any further arguments -
// - print envp in alphabetical order
//multiple arguments possible
//if key already exists - overwrite
//if not - create new node
//if no value - create empty key entry
//valid keys: letters, digits, underscores, but can't start with a digit

#include "builtin_internal.h"
#include "error.h"

static void	split_key(char *line, t_export_par *var, char *separator)
{
	char	*split_sign;

	split_sign = ft_strnstr(line, separator, ft_strlen(line));
	if (!split_sign)
	{
		var->key = ft_strdup(line);
		var->value = NULL;
		return ;
	}
	else
	{
		var->key = ft_substr(line, 0, split_sign - line);
		var->value = strdup(split_sign + ft_strlen(separator));
	}

}
// char	*ft_substr(char const *s, unsigned int start, size_t len)
//mallocs

static int	is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (('a' <= key[i] && key[i] <= 'z')
		|| ('A' <= key[i] && key[i] <= 'Z') || key[i] == '_')
		i++;
	else
		return (0);
	while (key[i])
	{
		if (('a' <= key[i] && key[i] <= 'z')
			|| ('A' <= key[i] && key[i] <= 'Z') || key[i] == '_'
			|| ('0' <= key[i] && key[i] <= '9'))
			i++;
		else
			return (0);
	}
	return (1);
}

static void	print_export_sorted(void)
{
	// ADD sort
	printf("oops, still need to finish this sorting function\n");
}

void output_declare(t_env *env)
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

int	bi_export(t_env *env, char **args)
{
	int				i;
	int				status;
	t_export_par	var;
	char			*obj;
	char			*str;
	int				is_valid;

	if (!args[1])
	{
		print_export_sorted();
		return (0);
	}
	status = 0;
	var.key = NULL;
	var.value = NULL;
	if (check_flags(env, args, &i) == ERROR)
		return (EXIT_MISUSE);
	while (args[i])
	{
		split_key(args[i], &var, "=");
		is_valid = is_valid_key(var.key);
		if (is_valid == 1)
			env->set_pair(env, var.key, var.value);
		else
		{
			split_key(args[i], &var, "+=");
			is_valid = is_valid_key(var.key);
			if (is_valid == 1)
			{
				str = env->get_value(env, var.key);
				if (str)
					str = ft_strdup(env->get_value(env, var.key));
				ft_strappend(&str, var.value);
				env->set_pair(env, var.key, str);
			}
		}
		if (is_valid == 0)
		{
			status = 1;
			obj = ft_strdup("export: ");
			if (!obj || ft_strappend(&obj, args[i]) != 1)
				output_error(MALLOC_ERROR, NULL);
			else
				output_error(obj, NOT_VALID_IDENTIFIER);
			if (obj)
				free(obj);
		}
		if (var.key)
			free(var.key);
		if (var.value)
			free(var.value);
		i++;
	}
	return (status);
}
