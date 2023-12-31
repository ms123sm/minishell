/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 10:04:49 by aajaanan          #+#    #+#             */
/*   Updated: 2023/08/26 13:30:49 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env_var	*env_var_new(char *key, char *value)
{
	t_env_var	*env_var_node;

	env_var_node = (t_env_var *)malloc(sizeof(t_env_var));
	if (!env_var_node)
		return (NULL);
	env_var_node->key = key;
	env_var_node->value = value;
	env_var_node->next = NULL;
	return (env_var_node);
}

int	env_var_update_value(t_env_var **env_var_list, t_env_var *new_node)
{
	t_env_var	*tmp;

	tmp = *env_var_list;
	while (tmp)
	{
		if (strcmp(tmp->key, new_node->key) == 0)
		{
			if (new_node->value == NULL)
				return (1);
			free(tmp->value);
			tmp->value = ft_strdup(new_node->value);
			free_env_var_node(new_node);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	env_var_insert_sorted(t_env_var **env_var_list, t_env_var *new_node)
{
	t_env_var	*p;
	t_env_var	*q;

	p = *env_var_list;
	q = NULL;
	if (!env_var_list || !new_node)
		return ;
	if (env_var_update_value(env_var_list, new_node))
		return ;
	if (!(*env_var_list))
	{
		*env_var_list = new_node;
		return ;
	}
	if (strcmp(new_node->key, (*env_var_list)->key) < 0)
	{
		new_node->next = *env_var_list;
		*env_var_list = new_node;
		return;
	}
	while (p && strcmp(p->key, new_node->key) < 0)
	{
		q = p;
		p = p->next;
	}
	q->next = new_node;
	new_node->next = p;
}

void	print_env(t_env_var *env_var_list)
{
	t_env_var	*tmp;

	tmp = env_var_list;
	while (tmp)
	{
		if (tmp->value)
			ft_printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

void	copy_env_to_list(char **envp, t_env_var **env_var_list)
{
	char		*value;
	char		**key_value;
	t_env_var	*new_node;     
	
	int i = 0;
	while (envp[i])
	{
		key_value = ft_split(envp[i], '=');
		// if (ft_strcmp(key_value[0], "OLDPWD") == 0)
		// {
		// 	new_node = env_var_new(ft_strdup(key_value[0]), NULL);
		// }
		// else
		// {
		// 	value = ft_strchr(envp[i], '=') + 1;
		// 	new_node = env_var_new(ft_strdup(key_value[0]), ft_strdup(value));
		// }
		
		value = ft_strchr(envp[i], '=') + 1;
		new_node = env_var_new(ft_strdup(key_value[0]), ft_strdup(value));
		
		env_var_insert_sorted(env_var_list, new_node);
		free_args(key_value);
		i++;
	}
}


void	free_env_var_node(t_env_var *node)
{
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

void	free_env_var_list(t_env_var *env_var_list)
{
	t_env_var	*current;
	t_env_var	*tmp;

	current = env_var_list;
	while (current)
	{
		tmp	= current;
		current = current->next;
		free_env_var_node(tmp);
	}
}


char *getenv_value(char *key, t_env_var *env_var_list)
{
	t_env_var	*tmp;

	tmp = env_var_list;
	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}