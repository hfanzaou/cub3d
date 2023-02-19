/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 23:08:20 by ajana             #+#    #+#             */
/*   Updated: 2023/02/19 06:04:03 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	set_tex(t_tex *texture, char *path, void *mlx_p)
{
	texture->tex_p = mlx_xpm_file_to_image(mlx_p, path, &(texture->width),
			&(texture->hight));
	if (!(texture->tex_p))
		return (ft_error("Invalid texture path\n"));
	texture->data = mlx_get_data_addr(texture->tex_p, &(texture->bpp),
			&(texture->size_line), &(texture->endian));
	if (!(texture->data))
		return (1);
	return (0);
}

int	textures_init(t_mlx *p)
{
	p->textures = malloc(sizeof(t_tex) * 4);
	if (!(p->textures))
		return (1);
	ft_memset(p->textures, 0, sizeof(t_tex) * 4);
	if (set_tex(p->textures, p->scene->north_tex, p->mlx_p)
		|| set_tex(p->textures + 1, p->scene->south_tex, p->mlx_p)
		|| set_tex(p->textures + 2, p->scene->east_tex, p->mlx_p)
		|| set_tex(p->textures + 3, p->scene->west_tex, p->mlx_p))
		return (1);
	return (0);
}

int	get_texture(t_cor *inter, t_ray *ray)
{
	if (inter->f)
	{
		if (ray->dir == 1 || ray->dir == 2)
			return (south);
		else if (ray->dir == 3 || ray->dir == 4)
			return (north);
	}
	else
	{
		if (ray->dir == 1 || ray->dir == 4)
			return (east);
		else if (ray->dir == 2 || ray->dir == 3)
			return (west);
	}
	return (-1);
}
