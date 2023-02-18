/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 23:09:38 by ajana             #+#    #+#             */
/*   Updated: 2023/02/17 23:32:08 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_byte(int n)
{
	if (n <= 255 && n >= 0)
		return (0);
	return (1);
}

int	get_elements(t_scene *scene)
{
	if (scene->north_tex && scene->south_tex && scene->west_tex
		&& scene->east_tex && scene->floor && scene->ceiling)
		return (1);
	return (0);
}

int rgb_int(int *res, char *line)
{
	char	**rgb;

	rgb = ft_split(line, ',');
	if (strlen2(rgb) != 3)
		return (ft_error("Invalid color format (R,G,B)\n"));
	if (is_byte(ft_atoi(*rgb)) || is_byte(ft_atoi(rgb[1])) || is_byte(ft_atoi(rgb[2])))
        return (1);
    else
    {
	    *res = (ft_atoi(*rgb) << 16 | ft_atoi(rgb[1]) << 8 | ft_atoi(rgb[2]));
        return (0);
    }
	ft_free(rgb);
}

int	identifier_check(char **iden_path, t_scene *scene)
{
    if (!strcmp(*iden_path, "NO"))
		scene->north_tex = iden_path[1];
    else if (!strcmp(*iden_path, "SO"))
        scene->south_tex = iden_path[1];
    else if (!strcmp(*iden_path, "EA"))
        scene->east_tex = iden_path[1];
    else if (!strcmp(*iden_path, "WE"))
        scene->west_tex = iden_path[1];
    else if (!strcmp(*iden_path, "F"))
        return (rgb_int(&scene->floor, iden_path[1]));
    else if (!strcmp(*iden_path, "C"))
        return (rgb_int(&scene->ceiling, iden_path[1]));
    else
        return (ft_error("Invalid identifier\n"));
    return (0);
}

int elements_check(char ***file, t_scene *scene)
{
    char    **split;

    while (**file)
    {
		if (!strcmp(**file, ""))
		{
			(*file)++;
			continue ;
		}
        split = ft_split(**file, ' ');
		if (strlen2(split) != 2)
			return (ft_error(NULL));
		if (identifier_check(split, scene))
			return (1);
		(*file)++;
		if (get_elements(scene))
			break ;
    }
	while ((**file) && !strcmp(**file, ""))
		(*file)++;
	return (0);
}
