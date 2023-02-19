/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 04:46:46 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/19 03:45:02 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int ac, char **av)
{
	t_mlx	*p;

	if (ac != 2)
		return (0);
	p = p_init(av[1]);
	if (!p)
		return (0);
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
