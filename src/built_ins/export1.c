/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:30:16 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 16:38:08 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include "error.h"

void	split_key(char *line, t_export_par *var, char *separator)
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
		var->value = ft_strdup(split_sign + ft_strlen(separator));
	}
}
// char	*ft_substr(char const *s, unsigned int start, size_t len)
//mallocs

int	is_valid_key(char *key)
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

void	sort_pointers_array(t_env_pair **array, int size)
{
	int			i;
	int			j;
	t_env_pair	*temp;

	j = 0;
	while (j < size)
	{
		i = 0;
		while (i < size - 1 - j)
		{
			if (ft_strcmp(array[i]->key, array[i + 1]->key) > 0)
			{
				temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;
			}
			i++;
		}
		j++;
	}
}

void	print_pointers_array(t_env_pair **array, int size)
{
	int	i;
	int	len;

	i = 0;
	while (i < size)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		len = ft_strlen(array[i]->key);
		write(STDOUT_FILENO, array[i]->key, len);
		if (array[i]->value)
			write(STDOUT_FILENO, "=", 1);
		{
			len = ft_strlen(array[i]->value);
			write(STDOUT_FILENO, array[i]->value, len);
		}
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

void	print_export_sorted(t_env *env)
{
	int			count;
	t_env_pair	**pointers_arr;
	t_env_pair	*cur;
	int			i;

	count = env->amount_vars(env);
	pointers_arr = malloc(sizeof(t_env_pair *) * count);
	if (!pointers_arr)
		return ;
	cur = env->head;
	i = 0;
	while (cur)
	{
		pointers_arr[i] = cur;
		cur = cur->next;
		i++;
	}
	sort_pointers_array(pointers_arr, count);
	print_pointers_array(pointers_arr, count);
	free(pointers_arr);
}
