/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 23:20:24 by ajana             #+#    #+#             */
/*   Updated: 2023/02/19 05:29:51 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_error(char *err_msg)
{
	ft_putstr_fd("Error\n", 2);
	if (err_msg)
		ft_putstr_fd(err_msg, 2);
	return (-1);
}

int	path_check(char *path)
{
	int	fd;

	if (ft_strcmp(&path[ft_strlen(path) - 4], ".cub"))
		return (ft_error("Invalid map extension\n"));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		ft_error("Error opening file\n");
	return (fd);
}

int	lines_count(char *path)
{
	int		tmp_fd;
	int		count;
	char	*line;

	count = 1;
	tmp_fd = open(path, O_RDONLY);
	line = get_next_line(tmp_fd);
	while (line)
	{
		free(line);
		line = NULL;
		count++;
		line = get_next_line(tmp_fd);
	}
	close(tmp_fd);
	return (count);
}

void	remove_newline(char **str)
{
	int	len;

	if (!str || !(*str))
		return ;
	len = ft_strlen(*str) - 1;
	if ((*str)[len] == '\n')
		(*str)[len] = '\0';
}

int	read_file(char *path, char **file, int lines_count)
{
	int		i;
	int		fd;

	fd = path_check(path);
	if (fd == -1)
		return (1);
	i = 0;
	while (i < lines_count)
	{
		file[i] = get_next_line(fd);
		remove_newline(&(file[i]));
		i++;
	}
	file[i] = NULL;
	while (i > 0 && is_empty_line(file[--i]))
	{
		free(file[i]);
		file[i] = NULL;
	}
	if (*file == NULL)
		return (ft_error("Empty file\n"));
	return (0);
}
