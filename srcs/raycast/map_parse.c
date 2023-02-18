/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 12:25:07 by idelfag           #+#    #+#             */
/*   Updated: 2023/02/18 00:03:47 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_free(char **ptr)
{
	while (*ptr)
	{
		free(*ptr);
		ptr++;
	}
}

int ft_error(char *err_msg)
{
    ft_putstr_fd("Error\n", 2);
    if (err_msg)
        ft_putstr_fd(err_msg, 2);
    return (-1);
}

int	strlen2(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
		i++;
	return (i);
}

int check_component(int c, int *player_pos)
{
    char    *valid_chars;
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

int first_nd_last(char  *line)
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

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == 0)
		return (1);
	return (0);
}

int	is_closed(char *line, char *up_line, char *down_line, int ind)
{
	if (line[ind] != '0')
		return (0);
	else if (is_space(line[ind + 1]) || is_space(line[ind - 1])
		|| is_space(up_line[ind]) || is_space(down_line[ind]))
		return (ft_error("Map must be closed\n"));
	return (0);
}

int	check_walls(char *line)
{
	int	line_len;
	int	i;

	if (!(*line))
		return (0);
	i = 0;
	line_len = ft_strlen(line);
	while (is_space(line[i]))
		i++;
	while (is_space(line[line_len - 1]))
		line_len--;
	if (line[i] != '1' || line[line_len - 1] != '1')
		return (ft_error("Map must be surrounded by walls\n"));
	return (0);
}

int mapline_check(char **file, t_scene *scene, int *player_pos)
{
	char	*line;
    char	c;
    int     i;

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

int map_check(char **file, t_scene *scene)
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
        scene->map[i] = *file;
		i++;
		file++;
    }
	scene->map[i] = NULL;
    return (0);
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
		if(len > width)
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
	printf("width: %d\nheight: %d\nplayer_dir %c\n", scene->map_w, scene->map_h, scene->player_dir);
	
}

t_scene	*map_parse(char *path)
{
    t_scene *scene;
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
	// print_scene(scene);
	write(1, "got herex", 10);
    return (scene);
}
