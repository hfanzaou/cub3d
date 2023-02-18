/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:49:17 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/18 09:50:28 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int get_color(t_tex *img, int x, int y)
{
  char *color;
  if (x >= img->width)
   x = img->width - 1;
  if (y >= img->hight)
    y = img->hight - 1; 
  color = &img->data[y * img->size_line + x * (img->bpp/8)];
  return (*(unsigned int *)color);
}

int draw_cf(t_mlx *p, int x)
{
  int i;
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

int get_x(t_mlx *p, t_cor *cor, float wall_hight, t_ray *ray)
{
  int x;
  float pos;
  int   tex_id;

  if (cor->f == 0)
    pos = cor->y - floor(cor->y / p->tile_size) * p->tile_size;
  else
    pos = cor->x - floor(cor->x / p->tile_size) * p->tile_size;
  tex_id = get_texture(cor, ray);
   if (tex_id == -1)
    return (-1);
  x = wall_hight * pos / p->tile_size;
  x = x / wall_hight * p->textures[tex_id].width;
  return (x);
}

int draw_wall(t_mlx *p, t_ray *ray, t_cor *cor)
{
  float proj_plane;
  float wall_hight;
  t_cor win_cor;
  t_cor tex_cor;
  float hight;
  int y2;
  int tex_id;

  proj_plane = (1200 / 2) / tan(p->fov / 2);
  wall_hight = round(p->tile_size / (ray->distance * cos(ray->ray - p->rot_angle)) * proj_plane);
  win_cor.x = ray->index;
  hight = wall_hight;
  tex_cor.x = get_x(p, cor, wall_hight, ray);
  if (tex_cor.x == -1)
    return (0);
  if (wall_hight >= 1200)
  {
    hight = 1199;
    y2 = (1200 / 2) - wall_hight / 2;
    win_cor.y = (1200 / 2) - 1199 / 2;
  }
  else
  {
    win_cor.y = (1200 / 2) - (wall_hight / 2);
    y2 = win_cor.y;
  }
  draw_cf(p, win_cor.x);
  tex_id = get_texture(cor, ray);
  while (win_cor.y < ((1200/ 2) + (hight / 2)))
  {
    tex_cor.y = (win_cor.y - y2) / wall_hight * p->textures[tex_id].hight;
    ((unsigned int *)p->xpm)[(int)win_cor.y * 1200 + (int)win_cor.x] = get_color(&(p->textures[tex_id]), (int)tex_cor.x, tex_cor.y);
    win_cor.y++;
  }
  return (0);
}