/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 12:25:07 by idelfag           #+#    #+#             */
/*   Updated: 2023/02/18 04:58:32 by hfanzaou         ###   ########.fr       */
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
	while (line[i] == ' ')
		i++;
	while (line[line_len - 1] == ' ')
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

int	is_byte(int n)
{
	if (n <= 255 && n >= 0)
		return (0);
	return (1);
}

int rgb_int(char *line)
{
    int		res;
	char	**rgb;

	res = 0;
	rgb = ft_split(line, ',');
	if (strlen2(rgb) != 3)
		return (ft_error("Invalid color format (R,G,B)\n"));
	if (is_byte(ft_atoi(*rgb)) || is_byte(ft_atoi(rgb[1])) || is_byte(ft_atoi(rgb[2])))
		return (ft_error("Colors must be in range [0,255]\n"));
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
	{
        scene->floor = rgb_int(iden_path[1]);
		if (scene->floor == -1)
			return (1);
	}
    else if (!strcmp(*iden_path, "C"))
	{
        scene->ceiling = rgb_int(iden_path[1]);
		if (scene->ceiling == -1)
			return (1);
	}
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
	printf("width: %d\nheight: %d\nplayer_dir %c\n", scene->map_w, scene->map_h, scene->player_dir);
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
