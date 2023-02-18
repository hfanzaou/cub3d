/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 23:20:24 by ajana             #+#    #+#             */
/*   Updated: 2023/02/17 23:21:49 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "get_next_line/get_next_line.h"

int	path_check(char *path)
{
    int fd;

    if (strcmp(&path[strlen(path) - 4], ".cub"))
        return (ft_error("invalid map format\n"));
    fd = open(path, O_RDONLY);
	if (fd == -1)
        ft_error("Error opening file\n");
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

char	**read_file(char *path)
{
	char	**file;
	int		count;
	int		i;
	int		fd;

	fd = path_check(path);
	if (fd == -1)
		return (NULL);
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