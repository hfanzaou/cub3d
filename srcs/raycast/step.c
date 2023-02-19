/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 03:06:21 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/19 06:04:33 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	ft_exit(void)
{
	exit(0);
}

int	check_collision(t_scene *map, int x, int y, int i)
{
	if (i == 1 && (map->map[(y + 1) / 50][x / 50] == '1')
		&& (map->map[(y) / 50][x / 50] == '1'))
		return (1);
	if (i == 1 && (map->map[y / 50][(x + 1) / 50] == '1')
		&& (map->map[(y) / 50][x / 50] == '1'))
		return (1);
	if (i == 1 && (map->map[y / 50][(x - 1) / 50] == '1')
		&& (map->map[(y) / 50][x / 50] == '1'))
		return (1);
	if (i == 1 && (map->map[(y - 1) / 50][(x) / 50] == '1')
		&& (map->map[(y) / 50][x / 50] == '0'))
		return (1);
	else if (i == 10)
	{
		if (map->map[y / 50][(x - i) / 50] == '1'
			|| map->map[y / 50][(x + i) / 50] == '1')
			return (1);
		else if (map->map[(y - i) / 50][x / 50] == '1'
			|| map->map[(y + i) / 50][x / 50] == '1')
			return (1);
	}
	return (0);
}

int	edit_cor(t_mlx *p)
{
	float	step;
	t_cor	pl;
	float	slide;

	slide = 0;
	step = 0;
	if (p->walk_dir == 0 && p->turn_dir == 0 && p->slide_dir == 0 && p->close)
		return (1);
	step = (p->walk_dir * p->walkspeed);
	slide = (p->slide_dir * p->walkspeed);
	p->rot_angle += (p->turn_dir * p->turnspeed);
	p->slide_angle = p->rot_angle - (M_PI / 2);
	pl.x = p->x + cos(p->rot_angle) * (step) + cos(p->slide_angle) * slide;
	pl.y = p->y + sin(p->rot_angle) * (step) + sin(p->slide_angle) * slide;
	if (check_collision(p->scene, (int)pl.x, (int)pl.y, 10))
	{
		p->rot_angle -= (p->turn_dir * p->turnspeed);
		return (1);
	}
	p->x = pl.x;
	p->y = pl.y;
	return (0);
}

int	step(void *ptr)
{
	t_mlx	*p;

	p = (t_mlx *)ptr;
	if (edit_cor(p))
		return (0);
	mlx_clear_window(p->mlx_p, p->mlx_win);
	ft_raycast(p);
	draw_mini(p);
	mlx_put_image_to_window(p->mlx_p, p->mlx_win, p->img, 0, 0);
	return (0);
}
