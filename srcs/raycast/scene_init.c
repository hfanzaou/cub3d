/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 12:25:07 by idelfag           #+#    #+#             */
/*   Updated: 2023/02/19 04:36:03 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (1);
	while (line[i])
	{
		if (!is_space(line[i]))
			return (0);
		i++;
	}
	return (1);
}

void	map_dimensions(t_scene *scene)
{
	int	i;
	int	width;
	int	len;

	i = 0;
	width = 0;
	scene->map_h = strlen2(scene->map);
	while ((scene->map)[i])
	{
		len = strlen((scene->map)[i]);
		if (len > width)
			width = len;
		i++;
	}
	scene->map_w = width;
}

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (1);
	while ((*s1 && *s2) && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	if (*s1 || *s2)
		return (1);
	return (0);
}

int	elements_check(char ***file, t_scene *scene)
{
	char	**split;

	scene->ceiling = -1;
	scene->floor = -1;
	while (**file)
	{
		if (!ft_strcmp(**file, "") || is_empty_line(**file))
		{
			(*file)++;
			continue ;
		}
		split = ft_split(**file, ' ');
		if (strlen2(split) != 2)
			return (ft_error("Invalid scene element\n"));
		if (identifier_check(split, scene))
			return (1);
		ft_free(split);
		free(split);
		(*file)++;
		if (got_all_elements(scene))
			break ;
	}
	while ((**file) && (!ft_strcmp(**file, "") || is_empty_line(**file)))
		(*file)++;
	return (0);
}

t_scene	*scene_parse(char *path)
{
	t_scene	*scene;
	char	**file;
	char	**to_free;
	int		count;

	count = lines_count(path);
	file = malloc ((count + 1) * sizeof(char *));
	if (!file || read_file(path, file, count))
		return (NULL);
	scene = malloc(sizeof(t_scene));
	ft_memset(scene, 0, sizeof(t_scene));
	to_free = file;
	if (elements_check(&file, scene))
		return (NULL);
	if (map_parse(file, scene))
		return (NULL);
	map_dimensions(scene);
	ft_free(to_free);
	free(to_free);
	return (scene);
}
