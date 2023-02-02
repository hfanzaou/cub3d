/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 04:46:46 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/02 02:39:56 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct s_mlx
{
  void  *mlx_p;
  void  *mlx_win;
  float   x;
  float   y;
  int   turn_dir;
  int     walk_dir;
  float   rot_angle;
  float   walkspeed;
  float   turnspeed;
} t_mlx;

typedef struct s_ray
{
  float ray;
  float vx_inter;
  float vy_inter;
  float hx_inter;
  float hy_inter;
  float dx;
  float dy;
  float tx;
  float ty;
  int whface;
} t_ray;
int worldMap[24][24]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int	ft_putstr_fd(char *str, int fd)
{
	while (*str)
	{
		write(fd, str, 1);
		str++;
	}
	return(1);
}
int	ft_error(int i)
{
	if (i == 1)
		return (ft_putstr_fd("Error initializing mlx", 2));
	return(1);	
}

t_ray *ft_norm(t_ray *ray)
{
  while (ray->ray > 2 * M_PI)
   ray->ray -= 2 * M_PI;
  if (ray->ray < 0)
    ray->ray = (2 * M_PI) + ray->ray; 
  return (ray);  
}

void  intersec(t_mlx *p, t_ray *ray)
{
  float angle;
  printf("%f\n", ray->ray);
  if (ray->whface == 1)
  {
    ray->vy_inter = floor((p->y) / 50) * 50 - 1;
    ray->vx_inter = p->x + ((ray->vy_inter - p->y)/tan(ray->ray));
    ray->dx = -((50/tan(ray->ray)));
    ray->dy = -50;
  }
  else if (ray->whface == 2)
  {
    angle = ray->ray; 
    ray->vy_inter = floor((p->y) / 50) * 50 - 1;
    ray->vx_inter = p->x + ((ray->vy_inter - p->y)/tan(angle));
    ray->dx = -((50/tan(angle)));
    ray->dy = -50;
  }
  else if (ray->whface == 3)
  {
    angle =ray->ray;
    ray->vy_inter = floor((p->y) / 50) * 50 + 50; 
    ray->vx_inter = p->x + ((ray->vy_inter - p->y)/tan(angle));
    ray->dx = ((50/tan(angle)));
    ray->dy = 50;
  }
  else if (ray->whface == 4)
  {
    angle = ray->ray;
    ray->vy_inter = floor((p->y / 50)) * 50 + 50;
    ray->vx_inter = p->x + ((ray->vy_inter - p->y)/tan(angle));
    ray->dx = ((50/tan(angle)));
    ray->dy = 50;
  }
}

void  intersec2(t_mlx *p, t_ray *ray)
{
  float angle;
  printf("%f\n", ray->ray);
  if (ray->whface == 1)
  {
    ray->hx_inter = floor((p->x) / 50) * 50 - 1;
    ray->hy_inter = p->y - ((p->x - ray->hx_inter) * tan(ray->ray));
    ray->ty = -((50 * tan(ray->ray)));
    ray->tx = -50;
  }
  else if (ray->whface == 2)
  {
    angle = 2 * M_PI - ray->ray; 
    ray->hx_inter = floor((p->x) / 50) * 50 + 50;
    ray->hy_inter = p->y - ((ray->hx_inter - p->x) / tan(angle));
    ray->ty = -((50 * tan(angle)));
    ray->tx = 50;
  }
  else if (ray->whface == 3)
  {
    angle = ray->ray;
    ray->hx_inter = floor((p->x) / 50) * 50 + 50; 
    ray->hy_inter = p->y + ((ray->hx_inter - p->x) * tan(angle));
    ray->ty = ((50 * tan(angle)));
    ray->tx = 50;
  }
  else if (ray->whface == 4)
  {
    angle = ray->ray;
    ray->hx_inter = floor((p->x / 50)) * 50 - 1;
    ray->hy_inter = p->y + ((p->x - ray->hx_inter) * tan(angle));
    ray->ty = ((50 * tan(angle)));
    ray->tx = -50;
  }
}
void  ft_hor(t_mlx *p, t_ray *ray, int f)
{
  int h;
  int w;
  w = 10;
  h = 10;
  void  *img;
  printf("whface = %d\n", ray->whface);
  if (f == 1)
    intersec(p, ray);
  else 
    intersec2(p, ray);  
  printf("px = %f\nhx = %f\nhy = %f\ntx = %f\nty = %f\n", p->x, ray->hx_inter, ray->hy_inter, ray->tx, ray->ty);
  img = mlx_xpm_file_to_image(p->mlx_p, "inter.xpm", &h, &w);
   if (ray->hy_inter > 1200 || ray->hy_inter < 0 || ray->hx_inter > 1200 || ray->hx_inter < 0)
      return ;
  while (worldMap[(int)ray->hy_inter/50][(int)ray->hx_inter/50] == 0)
  {
  
    mlx_put_image_to_window(p->mlx_p, p->mlx_win, img, ray->hx_inter + 4, ray->hy_inter);
    ray->hy_inter += ray->ty;
    ray->hx_inter += ray->tx;
    if (ray->hy_inter > 1200 || ray->hy_inter < 0 || ray->hx_inter > 1200 || ray->hx_inter < 0)
      break;
  }
  mlx_put_image_to_window(p->mlx_p, p->mlx_win, img, ray->hx_inter - 2, ray->hy_inter);
  mlx_destroy_image(p->mlx_p, img);
}
int raydir(float ray)
{
  if (ray >= 0 && ray <= M_PI / 2)
    return (1);
  if (ray > M_PI / 2 / 2 && ray <= M_PI)
    return (2);
  if (ray > M_PI && ray <= (3 * M_PI) / 2)
    return (3);
  if (ray > (3 * M_PI) / 2 && ray <= 2 * M_PI)
    return (4);
  return (0);        
}
void  castone(t_mlx *p, t_ray *ray)
{
  float x;
  float y;
  // float ay;
  // float ax;
  int i;
  i = 0;
  x = p->x + 5 - cos(ray->ray) * i;
  y = p->y + 5 - sin(ray->ray) * i;
  while (worldMap[(int)y / 50][(int)x / 50] == 0)
  {
    x = p->x + 5 - cos(ray->ray) * i;
    y = p->y + 5 - sin(ray->ray) * i;
    mlx_pixel_put(p->mlx_p, p->mlx_win, x, y, 16711680);
    i += 2;
  }
 ray = ft_norm(ray);
 //printf("ray = %f\n %f\n", ray->ray);
  ray->whface = raydir(ray->ray);
  //ft_hor(p, ray, 1);
  ft_hor(p, ray, 0);
}

int ft_raycast(t_mlx *p)
{
  float fov = 60 * (M_PI / 180);
  float i;
  t_ray *ray;
  ray = malloc(sizeof(t_ray));
  i = 0;
  ray->ray = p->rot_angle - (fov / 2);
//    while (i < 1200 / 10)
//  {
    castone(p, ray);
  //   ray->ray += fov / (1200 / 10);
  //   i++;
  // }
  return (0);
  
}
void  redraw(t_mlx *p)
{
  int i;
  int j;
  int h;
  int w;
  void *img_ptr;
  void *img_ptr2;
  j = 0;
  img_ptr = mlx_xpm_file_to_image(p->mlx_p, "file.xpm", &w, &h);
  img_ptr2 = mlx_xpm_file_to_image(p->mlx_p, "file2.xpm", &w, &h);
  while (j < 24)
  {
    i = 0;
    h = j * 50;
    while (i < 24)
    {
      w = i * 50;
      if (worldMap[j][i] != 0)
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr, i * 50, j * 50);
      else
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr2, i * 50, j * 50);  
      i++;
    }
    j++;
  }
  mlx_destroy_image(p->mlx_p, img_ptr);
  mlx_destroy_image(p->mlx_p, img_ptr2);
  img_ptr = mlx_xpm_file_to_image(p->mlx_p, "p.xpm", &w, &h);
  mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr, p->x, p->y);
  int k;
  k = 1;
  mlx_destroy_image(p->mlx_p, img_ptr);
  while (k <= 100)
  {
    mlx_pixel_put(p->mlx_p, p->mlx_win, p->x + 5 - cos(p->rot_angle) * k, p->y + 5 - sin(p->rot_angle) * k, 16711680);
    k++;
  }
  ft_raycast(p);
}

t_mlx *p_init(void *mlx_p, void *mlx_win, int x, int y)
{
    t_mlx *p;
 
    p = malloc(sizeof(t_mlx));
    p->mlx_p = mlx_p;
    p->mlx_win = mlx_win;
    p->x = x;
    p->y = y;
    p->walk_dir = 0;
    p->turn_dir = 0;
    p->rot_angle = M_PI / 2;
    p->turnspeed = 7 * (M_PI / 180);
    p->walkspeed = 5;
    return (p);
}

int  step(void *ptr)
{

  t_mlx *p;
  p = (t_mlx *)ptr;
  float step;
  float x;
  float y;
  if (p->walk_dir == 0 && p->turn_dir == 0)
    return 0;
  p->rot_angle += (p->turn_dir * p->turnspeed);
  step = (p->walk_dir * p->walkspeed);
  x = p->x + 5 + cos(p->rot_angle) * (step);
  y = p->y + 5 + sin(p->rot_angle) * (step);
  if (worldMap[(int)y / 50][(int)x / 50] != 0)
    return (0);
  p->x = x - 5;
  p->y = y - 5;
  mlx_clear_window(p->mlx_p, p->mlx_win);
  redraw(p);
  return (0);
}

int key_hook(int key, t_mlx *p)
{
  if ((key == 126 || key == 13))
    p->walk_dir = -1;
  if (key == 125 || key == 1)
    p->walk_dir = 1;
  if (key == 123 || key == 0)
    p->turn_dir = -1;
  if (key == 124 || key == 2)
    p->turn_dir = 1;
  return (0); 
}

int   key_hook2(int key, t_mlx *p)
{
   if (key == 126 || key == 13)
    p->walk_dir = 0;
 if (key == 125 || key == 1)
    p->walk_dir = 0;  
 if (key == 123 || key == 0)
    p->turn_dir = 0;
  if (key == 124 || key == 2)
    p->turn_dir = 0;
  return (0);
}

int mouse_hook(int x, int y, t_mlx *p)
{
  (void)y;
  if (x > 1200 / 2 + 200)
    p->turn_dir = 1;
  else if (x < 1200 / 2 - 200)
    p->turn_dir = -1;
  else
    p->turn_dir = 0;   
  return (0);    
}

void  redraw2(t_mlx *p)
{
  int i;
  int j;
  int h;
  int w;
  void *img_ptr;
  void *img_ptr2;
  j = 0;
  img_ptr = mlx_xpm_file_to_image(p->mlx_p, "file.xpm", &w, &h);
  img_ptr2 = mlx_xpm_file_to_image(p->mlx_p, "file2.xpm", &w, &h);
  while (j < 24)
  {
    i = 0;
    h = j * 50;
    while (i < 24)
    {
      w = i * 50;
      if (worldMap[j][i] != 0)
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr, i * 50, j * 50);
      else
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr2, i * 50, j * 50);  
      i++;
    }
    j++;
  }
  mlx_destroy_image(p->mlx_p, img_ptr);
  mlx_destroy_image(p->mlx_p, img_ptr2);
  img_ptr = mlx_xpm_file_to_image(p->mlx_p, "p.xpm", &w, &h);
  mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr, p->x, p->y);
  int k;
  k = 1;
  mlx_destroy_image(p->mlx_p, img_ptr);
  while (k <= 100)
  {
    mlx_pixel_put(p->mlx_p, p->mlx_win, p->x + 5 - cos(p->rot_angle) * k, p->y + 5 - sin(p->rot_angle) * k, 16711680);
    k++;
  }
  //ft_raycast(p);
}

int main()
{
	void	*mlx_p;
	void	*mlx_win;
  t_mlx *p;
  
	mlx_p = mlx_init();
	if (!mlx_p)
		return (ft_error(1));
	mlx_win = mlx_new_window(mlx_p, 50 * 24, 50 * 24, "cub3d");
	if (!mlx_win)
		return (ft_error(1));
  p = p_init(mlx_p, mlx_win, 19 * 35, 19 * 35);
  redraw2(p);
  mlx_hook(p->mlx_win, 2, 0, key_hook, p);
  mlx_hook(p->mlx_win, 3, 0, key_hook2, p);
  mlx_hook(p->mlx_win, 6, 0, mouse_hook, p);
  mlx_loop_hook(p->mlx_p, step, (void *)p);
	mlx_loop(mlx_p);
}
