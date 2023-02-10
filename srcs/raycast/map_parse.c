/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 12:25:07 by idelfag           #+#    #+#             */
/*   Updated: 2023/02/08 19:35:37 by ajana            ###   ########.fr       */
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

int check_component(int c)
{
    int     i;
    char    *valid_chars;

    i = 0;
    valid_chars = "01NSEW";
	if (c == '\n')
		return (0);
    while (valid_chars[i])
    {
        if (valid_chars[i] == c)
            return (i);
        i++;
    }
    return (-1);
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

int mapline_check(char *line, int *player_pos)
{
    int     i;
    int     component;

    i = 0;
    while (line[i])
    {
        if (line[i] == ' ')
        {
            i++;
            continue ;
        }
        if ((i == 0 || (!line[i + 1])) && line[i] != '1')
			return (1);
        component = check_component(line[i]);
        if (component == -1)
            return (ft_error("invalid map component\n"));
        else if (component > 1 && (*player_pos == 0))
            *player_pos = component;
		else if (component > 1 && (*player_pos))
			return (ft_error("player start position\n"));
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
		else if (mapline_check(*file, &player_pos))
			return (1);
        scene->map[i] = *file;
		i++;
		file++;
    }
	scene->map[i] = NULL;
    return (0);
}

int rgb_int(char *line)
{
    int		res;
	char	**rgb;

	rgb = ft_split(line, ',');
	res = 0;
	res = (ft_atoi(*rgb) << 16 | ft_atoi(rgb[1]) << 8 | ft_atoi(rgb[2]));
	ft_free(rgb);
	return (res);
}

int	get_elements(t_scene *scene)
{
	if (scene->north_tex && scene->south_tex && scene->west_tex
		&& scene->east_tex && scene->floor && scene->ceiling)
		return (1);
	return (0);
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
        scene->floor = rgb_int(iden_path[1]);
    else if (!strcmp(*iden_path, "C"))
        scene->ceiling = rgb_int(iden_path[1]);
    else
        return (1);
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
		if (split[2])
			return (1);
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

int	path_check(char *path)
{
    int fd;

    if (strcmp(&path[strlen(path) - 4], ".cub"))
        return (ft_error("invalid map format\n"));
    fd = open(path, O_RDONLY);
	if (fd == -1)
        perror("Error! ");
    return (fd);
}

int lines_count(char *path)
{
    int tmp_fd;
    int count;

    count = 0;
    tmp_fd = open(path, O_RDONLY);
    while (get_next_line(tmp_fd))
        count++;
	close(tmp_fd);
    return (count);
}

void	remove_newline(char **str)
{
	int	len;

	len = ft_strlen(*str) - 1;
	if ((*str)[len] == '\n')
		(*str)[len] = '\0';
}

char	**read_file(int fd, char *path)
{
	char	**file;
	int		count;
	int		i;

	i = 0;
	count = lines_count(path);
	file = malloc (sizeof(char *) * (count + 1));
	while (i < count)
	{
		file[i] = get_next_line(fd);
		remove_newline(&(file[i]));
		i++;
	}
	file[i] = NULL;
	return (file);
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
	printf("width: %d\nheight: %d\n", scene->map_w, scene->map_h);
}

t_scene	*map_parse(char *path)
{
    t_scene *scene;
	char	**file;
	int     fd;

	fd = path_check(path);
    if (fd == -1)
        return (NULL);
	file = read_file(fd, path);
    scene = malloc(sizeof(t_scene));
	ft_memset(scene, 0, sizeof(t_scene));
	if (elements_check(&file, scene))
		return (NULL);
    if (map_check(file, scene))
    	return (NULL);
	map_dimensions(scene);
	print_scene(scene);
    return (scene);
}
