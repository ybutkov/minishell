/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:35:12 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/02 18:12:58 by ashadrin         ###   ########.fr       */
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

int	bi_export(t_env *envp, char **args)
{
	int				i;
	t_export_par	*var;

	if (!args[1])
	{
		print_export_sorted();
		return (0);
	}
	var = NULL;
	i = 1;
	while(args[i])
	{
		split_key(args[i], var);
		if (is_valid_key(var->key))
			set_env_par(envp, var->key, var->value);
		else
			printf("minishell: export: '%s': not a valid identifier\n", args[i]);
		i++;
	}
	return (0);
}

void	split_key(char *line, t_export_par *var)
{
	char	*split_sign;

	split_sign = ft_strchr(line, '=');
	if (!split_sign)
	{
		var->key = ft_strdup(line);
		var->value = NULL;
		return ;
	}
	else
	{
		var->key = ft_substr(line, 0, split_sign - line);
		var->value = strdup(split_sign + 1);
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
	while(key[i])
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

void	print_export_sorted()
{
	printf("oops, still need to finish this sorting function\n");
}