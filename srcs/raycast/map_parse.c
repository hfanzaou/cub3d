/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 12:25:07 by idelfag           #+#    #+#             */
/*   Updated: 2023/02/02 02:49:21 by hfanzaou         ###   ########.fr       */
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

int first_nd_last(char  *line, int line_len)
{
	int	i;

	i = 0;
    while (line[i])
    {
		if (line[i] == '\n' && (i == line_len - 1))
			break ;
        if (line[i] != '1' && line[i] != ' ')
            return (ft_error("Map must be surrounded by walls\n"));
        i++;
    }
    return (0);
}

int mapline_check(char *line, int line_nb, int *player_pos)
{
    int     i;
    int     line_len;
    int     component;

    i = 0;
    line_len = strlen(line);
    if ((line_nb == 0) || line[line_len - 1] != '\n')
        return (first_nd_last(line, line_len));
    while (line[i])
    {
        if (line[i] == ' ')
        {
            i++;
            continue ;
        }
		if (line[i] == '\n' && (i == line_len - 1))
			break ;
        if ((i == 0 || i == line_len - 2) && line[i] != '1')
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
	int	line_len;

	i = 0;
	player_pos = 0;
	scene->map = malloc(sizeof(char *) * (strlen2(&(*file)) + 1));
    while (*file)
    {
		if (mapline_check(*file, i, &player_pos))
			return (1);
		line_len = strlen(*file);
		if ((*file)[line_len - 1] == '\n')
			(*file)[line_len - 1] = '\0';
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
	int	len;

	len = strlen(iden_path[1]);
	if (iden_path[1][len - 1] == '\n')
		iden_path[1][len - 1] = '\0';
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
	int		i;

	i = 0;
    while (**file)
    {
		if (!strcmp(**file, "\n"))
		{
			(*file)++;
			continue ;
		}
        split = ft_split(**file, ' ');
		if (split[2])
			return (0);
		if (identifier_check(split, scene))
			return (0);
		(*file)++;
		i++;
		if (get_elements(scene))
			break ;
    }
	while ((**file) && !strcmp(**file, "\n"))
		(*file)++;
	return (0);
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

char	**read_file(int fd, char *path)
{
	char	**file;
	int		count;
	int		i;

	i = 0;
	count = lines_count(path);
	file = malloc (sizeof(char *) * (count + 1));
	file[i] = get_next_line(fd);
	while (++i < count)
		file[i] = get_next_line(fd);
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
}

void	scene_init(t_scene *scene)
{
	scene->north_tex = NULL;
	scene->east_tex = NULL;
	scene->west_tex = NULL;
	scene->south_tex = NULL;
	scene->floor = 0;
	scene->ceiling = 0;
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
	scene_init(scene);
	if (elements_check(&file, scene))
		return (NULL);
    if (map_check(file, scene))
        return (NULL);
	print_scene(scene);
	// ft_free(file);
    return (scene);
}