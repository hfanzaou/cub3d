/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 23:09:38 by ajana             #+#    #+#             */
/*   Updated: 2023/02/19 02:37:01 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_byte(char *byte)
{
	int	n;
	int	i;

	i = 0;
	if (!byte || !(*byte))
		return (1);
	while (byte[i])
	{
		if (!ft_isdigit(byte[i++]))
			return (ft_error("Colors must be digits\n"));
	}
	n = ft_atoi(byte);
	if (n <= 255 && n >= 0)
		return (0);
	return (ft_error("R,G,B colors must be in range [0,255]\n"));
}

int	comma_count(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == ',')
			count++;
		i++;
	}
	return (count);
}

int	rgb_int(int *res, char *line)
{
	char	**rgb;

	if (*res != -1)
		return (ft_error("Duplicated color identifier\n"));
	else if (comma_count(line) != 2)
		return (ft_error("Invalid color format (R,G,B)\n"));
	rgb = ft_split(line, ',');
	if (strlen2(rgb) != 3)
		return (ft_error("Invalid color format (R,G,B)\n"));
	if (is_byte(*rgb) || is_byte(rgb[1]) || is_byte(rgb[2]))
	{
		ft_free(rgb);
		free(rgb);
		return (1);
	}
	else
	{
		*res = (ft_atoi(*rgb) << 16 | ft_atoi(rgb[1]) << 8 | ft_atoi(rgb[2]));
		ft_free(rgb);
		free(rgb);
		return (0);
	}
}

int	check_path(char *path, char **tex)
{
	int	fd;

	if (*tex)
		return (ft_error("Duplicated texture identifiers\n"));
	if (strcmp(&path[strlen(path) - 4], ".xpm"))
		return (ft_error("Invalid texture format\n"));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (ft_error("Invalid texture path\n"));
	close(fd);
	*tex = ft_strdup(path);
	return (0);
}

int	identifier_check(char **iden_path, t_scene *scene)
{
	if (!strcmp(*iden_path, "NO"))
		return (check_path(iden_path[1], &scene->north_tex));
	else if (!strcmp(*iden_path, "SO"))
		return (check_path(iden_path[1], &scene->south_tex));
	else if (!strcmp(*iden_path, "EA"))
		return (check_path(iden_path[1], &scene->east_tex));
	else if (!strcmp(*iden_path, "WE"))
		return (check_path(iden_path[1], &scene->west_tex));
	else if (!strcmp(*iden_path, "F"))
		return (rgb_int(&scene->floor, iden_path[1]));
	else if (!strcmp(*iden_path, "C"))
		return (rgb_int(&scene->ceiling, iden_path[1]));
	else
		return (ft_error("Invalid identifier\n"));
	return (0);
}

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

int	elements_check(char ***file, t_scene *scene)
{
	char	**split;

	scene->ceiling = -1;
	scene->floor = -1;
	while (**file)
	{
		if (!strcmp(**file, "") || is_empty_line(**file))
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
	while ((**file) && (!strcmp(**file, "") || is_empty_line(**file)))
		(*file)++;
	return (0);
}
