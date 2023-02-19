/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:55:09 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/19 06:04:27 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

t_ray	*ft_norm(t_ray *ray)
{
	while (ray->ray >= 2 * M_PI)
		ray->ray -= 2 * M_PI;
	while (ray->ray <= 0)
		ray->ray += (2 * M_PI);
	return (ray);
}

int	ft_dir(int dir, int f)
{
	if (f == 1 && (dir == 1 || dir == 2))
		return (-1);
	else if (f == 0 && (dir == 1 || dir == 4))
		return (-1);
	return (1);
}

int	raydir(float ray)
{
	if (ray >= 0 && ray <= M_PI / 2)
		return (1);
	if (ray > M_PI / 2 && ray <= M_PI)
		return (2);
	if (ray > M_PI && ray <= (3 * M_PI) / 2)
		return (3);
	if (ray > (3 * M_PI) / 2 && ray <= 2 * M_PI)
		return (4);
	return (0);
}

void	intersec(t_mlx *p, t_ray *ray)
{
	int	inter_dir;

	inter_dir = ft_dir(ray->dir, 1);
	if (inter_dir == -1)
	{
		ray->hvy_inter = floor((p->y) / p->tile_size) * p->tile_size - 1;
		ray->hvx_inter = p->x + ((ray->hvy_inter + 2 - p->y) / tan(ray->ray));
		ray->dx = -((p->tile_size / tan(ray->ray)));
		ray->dy = -p->tile_size;
	}
	else
	{
		ray->hvy_inter
			= floor((p->y) / p->tile_size) * p->tile_size + p->tile_size;
		ray->hvx_inter = p->x + ((ray->hvy_inter - p->y) / tan(ray->ray));
		ray->dx = ((p->tile_size / tan(ray->ray)));
		ray->dy = p->tile_size;
	}
}

void	intersec2(t_mlx *p, t_ray *ray)
{
	int	inter_dir;

	inter_dir = ft_dir(ray->dir, 0);
	if (inter_dir == -1)
	{
		ray->hvx_inter = floor((p->x) / p->tile_size) * p->tile_size - 1;
		ray->hvy_inter = p->y - ((p->x - ray->hvx_inter - 1) * tan(ray->ray));
		ray->dy = -((p->tile_size * tan(ray->ray)));
		ray->dx = -p->tile_size;
	}
	else
	{
		ray->hvx_inter
			= floor((p->x) / p->tile_size) * p->tile_size + p->tile_size;
		ray->hvy_inter = p->y - ((p->x - ray->hvx_inter) * tan(ray->ray));
		ray->dy = ((p->tile_size * tan(ray->ray)));
		ray->dx = p->tile_size;
	}
}
