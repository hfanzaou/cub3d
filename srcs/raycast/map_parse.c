/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 01:43:37 by ajana             #+#    #+#             */
/*   Updated: 2023/02/19 01:30:06 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_component(int c, int *player_pos)
{
	char	*valid_chars;
	char	*search;

	valid_chars = "01NSEW";
	search = ft_strchr(valid_chars, c);
	if (search)
	{
		if (ft_strlen(search) > 4)
			return (0);
		else if (*player_pos == 0)
		{
			*player_pos = 1;
			return (c);
		}
		else
			return (ft_error("Multiple player starting position\n"));
	}
	return (ft_error("Invalid map component\n"));
}

int	first_nd_last(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != '1' && line[i] != ' ')
			return (ft_error("Map must be surrounded by walls\n"));
		i++;
	}
	return (0);
}

int	check_walls(char *line)
{
	int	line_len;
	int	i;

	if (!line || is_empty_line(line))
		return (0);
	i = 0;
	line_len = ft_strlen(line);
	while (line[i] && is_space(line[i]))
		i++;
	while (line[i] && is_space(line[line_len - 1]))
		line_len--;
	if (line[i] != '1' || line[line_len - 1] != '1')
		return (ft_error("Map must be surrounded by walls\n"));
	return (0);
}

int	mapline_check(char **file, t_scene *scene, int *player_pos)
{
	char	*line;
	char	c;
	int		i;

	i = 0;
	line = *file;
	if (check_walls(line))
		return (1);
	while (line[i])
	{
		if (line[i] == ' ')
		{
			i++;
			continue ;
		}
		if (is_closed(line, *(file - 1), *(file + 1), i))
			return (1);
		c = check_component(line[i], player_pos);
		if (c == -1)
			return (1);
		else if (c)
			scene->player_dir = c;
		i++;
	}
	return (0);
}

int	map_parse(char **file, t_scene *scene)
{
	int	i;
	int	player_pos;

	i = 0;
	player_pos = 0;
	scene->map = malloc(sizeof(char *) * (strlen2(&(*file)) + 1));
	while (*file)
	{
		if ((!i || !(file + 1)) && (first_nd_last(*file)))
			return (1);
		else if (mapline_check(file, scene, &player_pos))
			return (1);
		scene->map[i] = ft_strdup(*file);
		i++;
		file++;
	}
	scene->map[i] = NULL;
	if (player_pos)
		return (0);
	ft_free(scene->map);
	return (ft_error("Player starting position not specified\n"));
}
