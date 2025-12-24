/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:16:25 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 04:08:14 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "shell_utils.h"
#include "envp_copy.h"
#include "constants.h"
#include "libft.h"

static void	free_ctx_content(t_ctx *ctx)
{
	if (!ctx)
		return ;
	if (ctx->envp)
		free_str_array(ctx->envp);
	if (ctx->env)
		ctx->env->free(ctx->env);
	free(ctx);
}

static char	**apply_func_to_array(char **arr, char *(*func)(const char *))
{
	int		i;
	char	**new_arr;

	if (!arr || !func)
		return (NULL);
	i = 0;
	while (arr[i])
		i++;
	new_arr = malloc((i + 1) * sizeof(char *));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		new_arr[i] = func(arr[i]);
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}

static int	setup_shlvl(t_env *env)
{
	char	*value;
	int		int_value;

	value = env->get_value(env, SHLVL);
	if (value)
		int_value = ft_atoi(value) + 1;
	else
		int_value = 1;
	value = ft_itoa(int_value);
	if (value == NULL)
		return (0);
	env->set_pair(env, SHLVL, value);
	free(value);
	return (1);
}

t_ctx	*create_ctx(char **envp)
{
	t_ctx	*ctx;

	ctx = malloc(sizeof(t_ctx));
	if (!ctx)
		return (HANDLE_ERROR_NULL);
	ctx->envp = apply_func_to_array(envp, ft_strdup);
	ctx->env = create_env();
	ctx->env->fill_from_array(ctx->env, envp);
	if (setup_shlvl(ctx->env) == 0)
		return (HANDLE_ERROR_NULL);
	ctx->last_exit_status = 0;
	ctx->should_exit = NO;
	ctx->free_ctx = free_ctx_content;
	return (ctx);
}
