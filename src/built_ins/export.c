/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:35:12 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 16:32:22 by ashadrin         ###   ########.fr       */
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

int	bi_export(t_env *env, char **args)
{
	int				i;
	int				status;
	t_export_par	var;
	int				is_valid;

	if (!args[1])
	{
		print_export_sorted(env);
		return (0);
	}
	status = 0;
	var.key = NULL;
	var.value = NULL;
	if (check_flags(env, args, &i) == ERROR)
		return (EXIT_MISUSE);
	while (args[i])
	{
		handle_plus_equal(env, &var, args[i], &is_valid);
		if (is_valid == 0)
			safe_error_output(&status, args[i]);
		key_value_free(&var);
		i++;
	}
	return (status);
}
