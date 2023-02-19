/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 05:45:57 by ajana             #+#    #+#             */
/*   Updated: 2023/02/19 01:55:59 by ajana            ###   ########.fr       */
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

int	strlen2(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
		i++;
	return (i);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == 0)
		return (1);
	return (0);
}

int	got_all_elements(t_scene *scene)
{
	if (scene->north_tex && scene->south_tex && scene->west_tex
		&& scene->east_tex && (scene->floor != -1) && (scene->ceiling != -1))
		return (1);
	return (0);
}

int	is_player_pos(char c)
{
	if (c == 'N' || c == 'E'
		|| c == 'S' || c == 'W')
		return (1);
	return (0);
}

int	is_closed(char *line, char *up_line, char *down_line, int ind)
{
	if (line[ind] != '0' && !is_player_pos(line[ind]))
		return (0);
	else if (is_space(line[ind + 1]) || is_space(line[ind - 1])
		|| is_space(up_line[ind]) || is_space(down_line[ind]))
		return (ft_error("Map must be closed\n"));
	return (0);
}
