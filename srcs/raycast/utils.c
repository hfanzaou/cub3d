/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:59:39 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/19 01:18:14 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
//======draw_utils=====//
int	get_color(t_tex *img, int x, int y)
{
	char	*color;

	if (x >= img->width)
		x = img->width - 1;
	if (y >= img->hight)
		y = img->hight - 1;
	color = &img->data[y * img->size_line + x * (img->bpp / 8)];
	return (*(unsigned int *)color);
}

int	get_x(t_mlx *p, t_cor *cor, float wall_hight, t_ray *ray)
{
	int		x;
	float	pos;
	int		tex_id;

	if (cor->f == 0)
		pos = cor->y - floor(cor->y / p->tile_size) * p->tile_size;
	else
		pos = cor->x - floor(cor->x / p->tile_size) * p->tile_size;
	tex_id = get_texture(cor, ray);
	if (tex_id == -1 || tex_id > 3)
		return (-1);
	x = wall_hight * pos / p->tile_size;
	x = x / wall_hight * p->textures[tex_id].width;
	return (x);
}

//=======raycast_utils======//

int	beyond_map(float ray_x, float ray_y, t_mlx *p)
{
	if (ray_y > p->scene->map_h * p->tile_size
		|| ray_y < 0
		|| ray_x > ft_strlen(p->scene->map[(int)ray_y / 50]) * p->tile_size
		|| ray_x < 0)
		return (1);
	return (0);
}

t_cor	*put_cor(float x, float y, t_cor *cor)
{
	cor->x = x;
	cor->y = y;
	return (cor);
}
