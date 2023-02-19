/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 23:24:38 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/19 06:09:21 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	redraw(t_mlx *p)
{
	int	i;
	int	j;

	j = 0;
	while (p->scene->map[j])
	{
		i = 0;
		while (p->scene->map[j][i])
		{
			if (ft_strchr("NSEW", p->scene->map[j][i]))
			{
				p->x = p->tile_size * i + p->tile_size / 2;
				p->y = p->tile_size * j + p->tile_size / 2;
				p->scene->map[j][i] = '0';
			}
			i++;
		}
		j++;
	}
}

int	step_init(t_mlx *p)
{
	float	step;
	float	x;
	float	y;
	float	slide;

	redraw(p);
	step = (p->walk_dir * p->walkspeed);
	slide = (p->slide_dir * p->walkspeed);
	p->rot_angle += (p->turn_dir * p->turnspeed);
	p->slide_angle = p->rot_angle - (M_PI / 2);
	x = p->x + cos(p->rot_angle) * (step) + cos(p->slide_angle) * slide;
	y = p->y + sin(p->rot_angle) * (step) + sin(p->slide_angle) * slide;
	if (check_collision(p->scene, (int)x, (int)y, 2))
	{
		p->rot_angle -= (p->turn_dir * p->turnspeed);
		return (0);
	}
	p->x = x;
	p->y = y;
	ft_raycast(p);
	draw_mini(p);
	mlx_put_image_to_window(p->mlx_p, p->mlx_win, p->img, 0, 0);
	return (0);
}

float	put_dir(t_mlx *p)
{
	if (p->scene->player_dir == 'N')
		return (M_PI / 2);
	if (p->scene->player_dir == 'S')
		return (M_PI * 3 / 2);
	if (p->scene->player_dir == 'W')
		return (0);
	if (p->scene->player_dir == 'E')
		return (M_PI);
	return (-1);
}

t_mlx	*p_init(char *path)
{
	t_mlx	*p;

	p = malloc(sizeof(t_mlx));
	if (!p)
		return (NULL);
	ft_memset(p, 0, sizeof(t_mlx));
	p->scene = scene_parse(path);
	if (!p->scene)
		return (NULL);
	p->rot_angle = put_dir(p);
	p->turnspeed = 4 * (M_PI / 180);
	p->walkspeed = 4;
	p->slide_angle = M_PI / 2;
	p->tile_size = 50;
	p->bpp = 1;
	return (p);
}
