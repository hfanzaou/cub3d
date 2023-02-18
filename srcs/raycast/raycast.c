/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:30:29 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/18 09:36:12 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_ray *ft_norm(t_ray *ray)
{
  while (ray->ray >= 2 * M_PI)
   ray->ray -= 2 * M_PI;
  while (ray->ray <= 0)
    ray->ray += (2 * M_PI);
  return (ray);
}

int ft_dir(int dir, int f)
{
  if (f == 1 && (dir == 1 || dir == 2))
    return (-1);
  else if (f == 0 && (dir == 1 || dir == 4))
    return (-1); 
  return (1);   
}

int raydir(float ray)
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

void  intersec(t_mlx *p, t_ray *ray)
{
  int inter_dir;
  inter_dir = ft_dir(ray->dir, 1);
  if (inter_dir == -1)
  {
    ray->hvy_inter = floor((p->y) / p->tile_size) * p->tile_size - 1;
    ray->hvx_inter = p->x + ((ray->hvy_inter - p->y)/tan(ray->ray)) + 1;
    ray->dx = -((p->tile_size/tan(ray->ray)));
    ray->dy = -p->tile_size;
  }
  else 
  {
    ray->hvy_inter = floor((p->y) / p->tile_size) * p->tile_size + p->tile_size; 
    ray->hvx_inter = p->x + ((ray->hvy_inter - p->y)/tan(ray->ray));
    ray->dx = ((p->tile_size/tan(ray->ray)));
    ray->dy = p->tile_size;
  }
}

void  intersec2(t_mlx *p, t_ray *ray)
{
  int inter_dir;
  inter_dir = ft_dir(ray->dir, 0);
  if (inter_dir == -1)
  {
    ray->hvx_inter = floor((p->x) / p->tile_size) * p->tile_size - 1;
    ray->hvy_inter = p->y - ((p->x - ray->hvx_inter) * tan(ray->ray)) + 0.5;
    ray->dy = -((p->tile_size * tan(ray->ray)));
    ray->dx = -p->tile_size;
  }
  else 
  {
    ray->hvx_inter = floor((p->x) / p->tile_size) * p->tile_size + p->tile_size; 
    ray->hvy_inter = p->y - ((p->x - ray->hvx_inter) * tan(ray->ray));
    ray->dy = ((p->tile_size * tan(ray->ray)));
    ray->dx = p->tile_size;
  }
}

t_cor *ft_hor(t_mlx *p, t_ray *ray, int f)
{
  int h;
  int w;
  t_cor *cor;
  cor = malloc(sizeof(t_cor));
  w = 10;
  h = 10;
  
  if (f == 1)
    intersec(p, ray);
  else 
    intersec2(p, ray);  
  if (ray->hvy_inter > p->scene->map_h * p->tile_size 
  || ray->hvy_inter < 0 || ray->hvx_inter > ft_strlen(p->scene->map[(int)ray->hvy_inter / 50]) * p->tile_size 
  || ray->hvx_inter < 0)
  {
    cor->x = ray->hvx_inter;
    cor->y = ray->hvy_inter;
    return (cor); 
  }   
  while (p->scene->map[(int)(ray->hvy_inter/50)][(int)(ray->hvx_inter/50)] == '0' 
  && !check_collision(p->scene, (int)ray->hvx_inter, (int)ray->hvy_inter, f))
  {
    ray->hvy_inter += ray->dy;
    ray->hvx_inter += ray->dx;
    if (ray->hvy_inter > p->scene->map_h * p->tile_size 
    || ray->hvy_inter < 0 || ray->hvx_inter > ft_strlen(p->scene->map[(int)ray->hvy_inter / 50]) * p->tile_size 
    || ray->hvx_inter < 0)
    {
      cor->x = ray->hvx_inter;
      cor->y = ray->hvy_inter;
      return (cor);
    }
  }
  cor->x = ray->hvx_inter;
  cor->y = ray->hvy_inter;
  return (cor);
}

t_cor  *castone(t_mlx *p, t_ray *ray)
{
  t_cor *h;
  t_cor *v;
  int i;
  i = 0;
  //draw_ray(p, ray);
 ray = ft_norm(ray);
  ray->dir = raydir(ray->ray);
  h = ft_hor(p, ray, 1);
  h->dis = sqrt((p->x - h->x) * (p->x - h->x) + (p->y - h->y) * (p->y - h->y));
  v = ft_hor(p, ray, 0);
  v->dis = sqrt((p->x - v->x) * (p->x - v->x) + (p->y - v->y) * (p->y - v->y));
  if (!v || !h)
    return NULL;
  if (h->dis > v->dis)
  {
    ray->distance = v->dis;
    v->f = 0;
    free(h);
    return (v);
  }
  else 
  {
    ray->distance = h->dis; 
    h->f = 1;
    free(v);
    return (h);  
  }
}

void ft_raycast(t_mlx *p)
{
  int i;
  t_cor *cor;
  
  p->fov = 60 * (M_PI / 180);
  t_ray *ray;
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