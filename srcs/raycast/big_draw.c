/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:49:17 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/18 22:22:07 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	draw_cf(t_mlx *p, int x)
{
	int	i;

	i = 0;
	while (i < 1200)
	{
		if (i < 1200 / 2)
			((unsigned int *)p->xpm)[i * 1200 + x] = p->scene->ceiling;
		else
			((unsigned int *)p->xpm)[i * 1200 + x] = p->scene->floor;
		i++;
	}
	return (0);
}

float	fix_wall(float wall_hight, t_cor *win_cor)
{
	float	hight;

	hight = wall_hight;
	if (wall_hight >= 1200)
	{
		hight = 1199;
		win_cor->dis = (1200 / 2) - wall_hight / 2;
		win_cor->y = (1200 / 2) - 1199 / 2;
	}
	else
	{
		win_cor->y = (1200 / 2) - (wall_hight / 2);
		win_cor->dis = win_cor->y;
	}
	return (hight);
}

void	put_wall(t_mlx *p, t_cor *win_cor, t_cor *tex_cor, float wall_hight)
{
	while (win_cor->y < ((1200 / 2) + (tex_cor->dis / 2)))
	{
		tex_cor->y = (win_cor->y - win_cor->dis)
			/ wall_hight * p->textures[tex_cor->f].hight;
		((unsigned int *)p->xpm)[(int)win_cor->y * 1200 + (int)win_cor->x]
			= get_color(&(p->textures[tex_cor->f]),
				(int)tex_cor->x, tex_cor->y);
		win_cor->y++;
	}
}

int	draw_wall(t_mlx *p, t_ray *ray, t_cor *cor)
{
	float	proj_plane;
	float	wall_hight;
	t_cor	*win_cor;
	t_cor	*tex_cor;

	win_cor = malloc(sizeof(t_cor));
	tex_cor = malloc(sizeof(t_cor));
	if (!win_cor || !tex_cor)
		exit(1);
	proj_plane = (1200 / 2) / tan(p->fov / 2);
	wall_hight = round(p->tile_size
			/ (ray->distance * cos(ray->ray - p->rot_angle)) * proj_plane);
	win_cor->x = ray->index;
	tex_cor->x = get_x(p, cor, wall_hight, ray);
	if (tex_cor->x == -1)
		return (0);
	tex_cor->dis = fix_wall(wall_hight, win_cor);
	draw_cf(p, win_cor->x);
	tex_cor->f = get_texture(cor, ray);
	put_wall(p, win_cor, tex_cor, wall_hight);
	free(win_cor);
	free(tex_cor);
	return (0);
}
