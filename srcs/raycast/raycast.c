/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:30:29 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/19 05:22:55 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_cor	*ft_hor(t_mlx *p, t_ray *ray, int f)
{
	t_cor	*cor;

	cor = malloc(sizeof(t_cor));
	if (!cor)
		exit(1);
	if (f == 1)
		intersec(p, ray);
	else
		intersec2(p, ray);
	if (beyond_map(ray->hvx_inter, ray->hvy_inter, p))
		return (put_cor(ray->hvx_inter - 1, ray->hvy_inter - 1, cor));
	while (p->scene->map
		[(int)(ray->hvy_inter / 50)][(int)(ray->hvx_inter / 50)] == '0'
		&& !check_collision(p->scene, (int)ray->hvx_inter,
		(int)ray->hvy_inter, 1))
	{
		ray->hvy_inter += ray->dy;
		ray->hvx_inter += ray->dx;
		if (beyond_map(ray->hvx_inter, ray->hvy_inter, p))
			return (put_cor(ray->hvx_inter, ray->hvy_inter, cor));
	}
	return (put_cor(ray->hvx_inter, ray->hvy_inter, cor));
}

t_cor	*chose_inter(t_cor *h, t_cor *v, t_ray *ray)
{
	if (h->dis > v->dis)
	{
		ray->distance = v->dis;
		v->f = 0;
		free(h);
		return (v);
	}
	else if (h->dis <= v->dis)
	{
		ray->distance = h->dis;
		h->f = 1;
		free(v);
		return (h);
	}
	return (NULL);
}

t_cor	*castone(t_mlx *p, t_ray *ray)
{
	t_cor	*h;
	t_cor	*v;

	ray = ft_norm(ray);
	ray->dir = raydir(ray->ray);
	h = ft_hor(p, ray, 1);
	h->dis
		= sqrt((p->x - h->x) * (p->x - h->x) + (p->y - h->y) * (p->y - h->y));
	v = ft_hor(p, ray, 0);
	v->dis
		= sqrt((p->x - v->x) * (p->x - v->x) + (p->y - v->y) * (p->y - v->y));
	if (!v || !h)
		exit(0);
	return (chose_inter(h, v, ray));
}

void	ft_raycast(t_mlx *p)
{
	int		i;
	t_cor	*cor;
	t_ray	*ray;

	p->fov = 60 * (M_PI / 180);
	ray = malloc(sizeof(t_ray));
	i = 0;
	ray->ray = p->rot_angle - (p->fov / 2);
	ray->index = 0;
	while (i < 1200)
	{
		cor = castone(p, ray);
		ray->ray += ((p->fov) / (1200));
		ray->index = i;
		draw_wall(p, ray, cor);
		free(cor);
		i++;
	}
	free(ray);
}
