/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 04:46:46 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/19 02:41:56 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	fun_free(t_mlx *p)
{
	free(p);
	return (0);
}

int	ft_exit(void)
{
	exit(0);
}

int	ft_close(int keycode, t_mlx *p)
{
	(void)p;
	if (keycode == 53)
	{
		p->close = 1;
		ft_exit();
	}
	else
		return (0);
	return (0);
}

int	check_collision(t_scene *map, int x, int y, int i)
{
	if (i == 1 && (map->map[(y + 1) / 50][x / 50] == '1'))
		return (1);
	if (i == 1 && (map->map[y / 50][(x + 1) / 50] == '1'))
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

// int draw_ray(t_mlx *p, float angle)
// {
//   float x;
//   float y;
//   int i;
//   int j;

//   j = 0;
//   i = 0;
//     while (j < 1200)
//     {
//       i = 0;
//       x = p->x - cos(angle) * i;
//       y = p->y - sin(angle) * i;
//       while (p->scene->map[(int)y 
//				/ p->tile_size][(int)x / p->tile_size] == '0')
//       {
//         x = p->x - cos(angle) * i;
//         y = p->y - sin(angle) * i;
//         ((unsigned int *)p->xpm)[((int)x / 20) * p->size_line 
//				+ ((int)y / 20) * (p->bpp / 8)] = 16711680;
//         //mlx_pixel_put(p->mlx_p, p->mlx_win, x/5, y/5, 16711680);
//         i++;
//       }
//        angle += ((p->fov) / (1200));
//       j++;
//     }
//   return (0);
// }

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

int	main(int ac, char **av)
{
	t_mlx	*p;

	if (ac != 2)
		return (0);
	p = p_init(av[1]);
	if (!p)
		return (1);
	p->mlx_p = mlx_init();
	p->mlx_win = mlx_new_window(p->mlx_p, 1200, 1200, "cub3d");
	if (!(p->mlx_p) || !(p->mlx_win))
		return (ft_error("Error initializing mlx\n"));
	p->img = mlx_new_image(p->mlx_p, 1200, 1200);
	p->xpm = mlx_get_data_addr(p->img, &p->bpp, &p->size_line, &p->endian);
	if (textures_init(p))
		return (1);
	step_init(p);
	mlx_hook(p->mlx_win, 2, 0, key_hook, p);
	mlx_hook(p->mlx_win, 3, 0, key_hook2, p);
	mlx_hook(p->mlx_win, 6, 0, mouse_hook, p);
	mlx_loop_hook(p->mlx_p, step, (void *)p);
	mlx_hook(p->mlx_win, 17, 0, ft_exit, &p);
	mlx_loop(p->mlx_p);
}
