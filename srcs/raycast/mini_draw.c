/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_draw.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:36:39 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/18 23:44:30 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	fill_square(t_mlx *p, int x, int y, int r)
{
	int	i;
	int	j;

	i = 0;
	while (i < r * 2)
	{
		j = 0;
		while (j < r * 2)
		{
			if (i == 0 || j == 0 || i == r * 2 - 1 || j == r * 2 - 1)
				((unsigned int *)p->xpm)[(y / r + i) * 1200 + x / r + j]
					= 0x000000;
			else if (p->scene->map[y / p->tile_size][x / p->tile_size] == '0')
				((unsigned int *)p->xpm)[(y / r + i) * 1200 + x / r + j]
					= 0x808080;
			else
				((unsigned int *)p->xpm)[(y / r + i) * 1200 + x / r + j]
					= 0xFFFFFF;  
			j += 2;
		}
		i += 2;
	}
}

void	fill_player(t_mlx *p, int x, int y, int r)
{
	int	i;
	int	j;

	i = -2;
	j = -2;
	while (i < 2)
	{
		j = -2;
		while (j < 2)
		{
			((unsigned int *)p->xpm)[(y / r + i) * 1200 + x/r + j] = 0xFF0000;
			j++;
		}
		i++;
	}
}

void fill_all(t_mlx *p, int i, int j, int r)
{
  if (p->scene->map[j][i] == '1')
    fill_square(p, i * p->tile_size, j * p->tile_size, r);
  else if (p->scene->map[j][i] == '0')
    fill_square(p, i * p->tile_size, j * p->tile_size, r);
  if (ft_strchr("NSEW", p->scene->map[j][i]))
  {
    fill_square(p, i * p->tile_size, j * p->tile_size, r);
    fill_player(p, i * p->tile_size, j * p->tile_size, r);
    p->scene->map[j][i] = '0';
  } 
}

int draw_mini(t_mlx *p)
{
  char **map;
  int j;
  int i;
  int r;
  
  j = 0;
  map = p->scene->map;
  r = 5;
  while (p->tile_size / r * p->scene->map_h > 1200 || p->tile_size / r * p->scene->map_w > 1200)
    r++;
  while (map[j])
  {
    i = 0;
    while (map[j][i])
    {
      fill_all(p, i, j, r);
      i++;
    }
    j++;
  }
  fill_player(p, p->x, p->y, r);
  return (0);
}
