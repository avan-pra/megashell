/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: velovo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 18:01:39 by velovo            #+#    #+#             */
/*   Updated: 2020/07/16 18:01:53 by velovo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../head/minishell.h"

/*
**	copy part of the string typed by the user to the given split
*/

static void	*fill_double_quote(t_word *giv, const char *str, t_env *enviro, int c_doll)
{
	++*giv->i;
	while (str[*giv->i] != '\"' && str[*giv->i] != '\0')
	{
		if (str[*giv->i] == '\\' && str[*giv->i + 1] == '\"')
		{
			++*giv->i;
			giv->word[*giv->dep] = str[*giv->i];
			++*giv->dep;
			++*giv->i;															//bof qwa
		}
		else if (str[*giv->i] == '$' && c_doll == '$' && str[*giv->i + 1] != ' '
			&& str[*giv->i + 1] != '\0' && str[*giv->i + 1] != '$'
			&& str[*giv->i + 1] != '\\' && str[*giv->i + 1] != '\"' && str[*giv->i + 1] != '\'')
		{
			if ((giv->word = fill_with_enviro(str, *enviro, giv)) == NULL)
				return (NULL);
			++*giv->i;
		}
		else
		{
			giv->word[*giv->dep] = str[*giv->i];
			++*giv->dep;
			++*giv->i;
		}
	}
	return (giv->word);
}

static void	*fill_single_quote(t_word *giv, const char *str, t_env *enviro, int c_doll)
{
	++*giv->i;
	while (str[*giv->i] != '\'' && str[*giv->i] != '\0')
	{
		if (str[*giv->i] == '$' && c_doll == '$' && str[*giv->i + 1] != ' '
			&& str[*giv->i + 1] != '\0' && str[*giv->i + 1] != '$'
			&& str[*giv->i + 1] != '\\' && str[*giv->i + 1] != '\'' && str[*giv->i + 1] != '\"')
		{
			if ((giv->word = fill_with_enviro(str, *enviro, giv)) == NULL)
				return (NULL);
			++*giv->i;
		}
		else
		{
			giv->word[*giv->dep] = str[*giv->i];
			++*giv->dep;
			++*giv->i;
		}
	}
	return (giv->word);
}

static void	*fill_default(t_word *giv, const char *str, t_env *enviro, int c_doll)
{
	if (str[*giv->i] == '$' && c_doll == '$' && str[*giv->i + 1] != ' '
		&& str[*giv->i + 1] != '\0'
		&& str[*giv->i + 1] != '$' && str[*giv->i + 1] != '\\')
	{
		if ((giv->word = fill_with_enviro(str, *enviro, giv)) == NULL)
			return (NULL);
	}
	else
	{
		giv->word[*giv->dep] = str[*giv->i];
		++*giv->dep;
	}
	return (giv->word);
}

static void	*fill_loop(t_word *giv, const char *str, t_env *enviro, int c_doll)
{
	if (str[*giv->i] == '\"')
	{
		if (fill_double_quote(giv, str, enviro, c_doll) == NULL)
			return (NULL);
	}
	else if (str[*giv->i] == '\'')
	{
		if (fill_single_quote(giv, str, enviro, c_doll) == NULL)
			return (NULL);
	}
	else if (str[*giv->i] == '\\')
	{
		++*giv->i;
		giv->word[*giv->dep] = str[*giv->i];
		++*giv->dep;
	}
	else
	{
		if (fill_default(giv, str, enviro, c_doll) == NULL)
			return (NULL);
	}
	++*giv->i;
	return (giv->word);
}

char		*fill_word(const char *str, t_env *enviro, int *c_split, int c_doll)
{
	int		i;
	int		dep;
	char	*word;
	t_word	giv;

	giv.c_split = c_split;
	giv.dep = &dep;
	giv.i = &i;
	giv.word = word;
	word_setup(str, &giv);
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '<' && str[i] != '>')
	{
		if (fill_loop(&giv, str, enviro, c_doll) == NULL)
			return (NULL);
	}
	giv.word[dep] = '\0';
	return (giv.word);
}
