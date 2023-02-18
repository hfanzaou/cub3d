/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 12:25:07 by idelfag           #+#    #+#             */
/*   Updated: 2023/02/18 06:21:05 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	print_scene(t_scene *scene)
{
	int	i;

	i = 0;
	if (!scene)
	{
		printf("null");
		return ;
	}
	printf("no: %s\nso: %s\nwe: %s\nea: %s\nfloor: %x\nceiling: %x\n",
		scene->north_tex, scene->south_tex, scene->west_tex,
		scene->east_tex, scene->floor, scene->ceiling);
	while (scene->map[i])
		printf("%s\n", scene->map[i++]);
	printf("width: %d\nheight: %d\nplayer_dir %c\n",
		scene->map_w, scene->map_h, scene->player_dir);
}

t_scene	*map_parse(char *path)
{
	t_scene	*scene;
	char	**file;

	file = read_file(path);
	if (!file)
		return (NULL);
	scene = malloc(sizeof(t_scene));
	ft_memset(scene, 0, sizeof(t_scene));
	if (elements_check(&file, scene))
		return (NULL);
	if (map_check(file, scene))
		return (NULL);
	map_dimensions(scene);
	return (scene);
}
