/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 04:46:46 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/08 21:27:16 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
  t_scene *scene;
} t_mlx;

typedef struct s_ray
{
  float ray;
  float hvx_inter;
  float hvy_inter;
  float dx;
  float dy;
  int dir;
  int f;
} t_ray;

typedef struct s_cor
{
  int x;
  int y;
}  t_cor;

// int	ft_putstr_fd(char *str, int fd)
// {
// 	while (*str)
// 	{
// 		write(fd, str, 1);
// 		str++;
// 	}
// 	return(1);
// }

int	ft_error2(int i)
{
	if (i == 1)
		ft_putstr_fd("Error initializing mlx", 2);
	return(1);	
}

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
  return (0);   
}

void  intersec(t_cor *p, t_ray *ray)
{
  int inter_dir;
  printf("%f\n", ray->ray);
  inter_dir = ft_dir(ray->dir, 1);
  if (inter_dir == -1)
  {
    ray->hvy_inter = floor((p->y) / 50) * 50 - 1;
    ray->hvx_inter = p->x + ((ray->hvy_inter - p->y)/tan(ray->ray));
    ray->dx = -((50/tan(ray->ray)));
    ray->dy = -50;
  }
  else 
  {
    ray->hvy_inter = floor((p->y) / 50) * 50 + 50; 
    ray->hvx_inter = p->x + ((ray->hvy_inter - p->y)/tan(ray->ray));
    ray->dx = ((50/tan(ray->ray)));
    ray->dy = 50;
  }
}

void  intersec2(t_cor *p, t_ray *ray)
{
  int inter_dir;
  inter_dir = ft_dir(ray->dir, 0);
  printf("%f\n", ray->ray);
  if (inter_dir == -1)
  {
    ray->hvx_inter = floor((p->x) / 50) * 50 - 1;
    ray->hvy_inter = p->y - ((p->x - ray->hvx_inter) * tan(ray->ray));
    ray->dy = -((50 * tan(ray->ray)));
    ray->dx = -50;
  }
  else 
  {
    ray->hvx_inter = floor((p->x) / 50) * 50 + 50; 
    ray->hvy_inter = p->y - ((p->x - ray->hvx_inter) * tan(ray->ray));
    ray->dy = ((50 * tan(ray->ray)));
    ray->dx = 50;
  }
}

void put_dot(t_mlx *p, int x, int y)
{
  int i;
  int j;
  i = 0;
  j = 0;
  while (i < 6)
  {
    j = 0;
    while (j < 6)
    {
      mlx_pixel_put(p->mlx_p, p->mlx_win, x + i, y + j, 16711680);
      j++;
    }
    i++;
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
  
  cor->x = p->x;
  cor->y = p->y;
  printf("whface = %d\n", ray->dir);
  if (f == 1)
    intersec(cor, ray);
  else 
    intersec2(cor, ray);  
  printf("px = %f\npy = %f\nhx = %f\nhy = %f\ntx = %f\nty = %f\n", p->x, p->y, ray->hvx_inter, ray->hvy_inter, ray->dx, ray->dy);
  if (ray->hvy_inter > p->scene->map_h * 50 || ray->hvy_inter < 0 || ray->hvx_inter > p->scene->map_w * 50 || ray->hvx_inter < 0)
      {
        cor->x = ray->hvx_inter;
        cor->y = ray->hvy_inter;
      return (cor); 
      }   
  while (p->scene->map[(int)ray->hvy_inter/50][(int)ray->hvx_inter/50] == '0' && p->scene->map[(int)(ray->hvy_inter + 1)/50][(int)(ray->hvx_inter + 1)/50] == '0')
  {
    //put_dot(p, ray->hvx_inter, ray->hvy_inter);
    ray->hvy_inter += ray->dy;
    ray->hvx_inter += ray->dx;
    if (ray->hvy_inter > p->scene->map_h * 50 || ray->hvy_inter < 0 || ray->hvx_inter > p->scene->map_w * 50 || ray->hvx_inter < 0)
    {
       cor->x = ray->hvx_inter;
      cor->y = ray->hvy_inter;
      return (cor);
    }
  }
  //put_dot(p, ray->hvx_inter, ray->hvy_inter)
    cor->x = ray->hvx_inter;
    cor->y = ray->hvy_inter;
  // else
  // {
  //   cor->x = ray->hvx_inter + (p->x - ray->hvx_inter);
  //   cor->y = ray->hvy_inter + (p->y - ray->hvy_inter);
  // }
  return (cor);
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

void  castone(t_mlx *p, t_ray *ray)
{
  float x;
  float y;
  int hypotv = -1;
  t_cor *h;
  t_cor *v;
  // float ay;
  // float ax;
  int i;
  i = 1;
    x = p->x + 5 - cos(ray->ray) * i;
    y = p->y + 5 - sin(ray->ray) * i;
  // else
  // {
  //   x = p->x + 6 - cos(ray->ray) * i;
  //   y = p->x + 6 - sin(ray->ray) * i;
  // }
  while (p->scene->map[(int)y / 50][(int)x / 50] == '0')
  {
    x = p->x + 5 - cos(ray->ray) * i;
    y = p->y + 5 - sin(ray->ray) * i;
   //mlx_pixel_put(p->mlx_p, p->mlx_win, x, y, 16711680);
    i += 2;
  }
  // if (p->scene->map[(int)p->x / 50][(int)p->y / 50] == '1')
  // {  
  //   p->x = p->x;
  //   p->y = p->y;
  // }
 ray = ft_norm(ray);
 printf("ray = %f\n", ray->ray);
 //printf("ray = %f\n %f\n", ray->ray);
  ray->dir = raydir(ray->ray);
  h = ft_hor(p, ray, 1);
  (void)v;
  v = ft_hor(p, ray, 0);
  if (!v || !h)
    return ;
  printf("hypotv = %f\n", hypot(v->x, v->y));
  printf("hypoth = %f\n", hypot(h->x, h->y));  
  if (ray->dir == 1)  
    hypotv = h->x >= v->x && h->y >= v->y;
  else if (ray->dir == 2)
    hypotv = v->x >= h->x && h->y >= v->y;
  else if (ray->dir == 3)
    hypotv = v->x >= h->x && v->y >= h->y;
  else if (ray->dir == 4)
    hypotv =  v->x <= h->x && v->y >= h->y; 
  if (hypotv)
    put_dot(p, h->x, h->y);
  else 
    put_dot(p, v->x, v->y);  
}

int ft_raycast(t_mlx *p)
{
  float fov = 60 * (M_PI / 180);
  float i;
  t_ray *ray;
  ray = malloc(sizeof(t_ray));
  i = 0;
  ray->ray = p->rot_angle - (fov / 2);
  while (i < 1200 / 4)
   {
    castone(p, ray);
    ray->ray += fov / (1200 / 4);
    i++;
  }
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
  void *img_ptr3;
  j = 0;
  img_ptr = mlx_xpm_file_to_image(p->mlx_p, "file.xpm", &w, &h);
  img_ptr2 = mlx_xpm_file_to_image(p->mlx_p, "file2.xpm", &w, &h);
  img_ptr3 = mlx_xpm_file_to_image(p->mlx_p, "p.xpm", &w, &h);
  while (p->scene->map[j])
  {
    i = 0;
    h = j * 50;
    while (p->scene->map[j][i])
    {
      w = i * 50;
      if (p->scene->map[j][i] == '1')
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr, i * 50, j * 50);
      else if (p->scene->map[j][i] == '0')
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr2, i * 50, j * 50);
      if (ft_strchr("NSEW", p->scene->map[j][i]))
      {
        p->x = 50 * i + 25;
        p->y = 50 * j + 25;
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr2, i * 50, j * 50);
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr3, p->x, p->y);
        p->scene->map[j][i] = '0';
      }    
      i++;
    }
    j++;
  }
  mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr3, p->x, p->y);
  mlx_destroy_image(p->mlx_p, img_ptr);
  mlx_destroy_image(p->mlx_p, img_ptr2);
  mlx_destroy_image(p->mlx_p, img_ptr3);
  int k;
  k = 1;
  while (k <= 100)
  {
    mlx_pixel_put(p->mlx_p, p->mlx_win, p->x + 5 - cos(p->rot_angle) * k, p->y + 5 - sin(p->rot_angle) * k, 16711680);
    k++;
  }
  ft_raycast(p);
}

t_mlx *p_init()
{
    t_mlx *p;
 
    p = malloc(sizeof(t_mlx));
    p->x = 0;
    p->y = 0;
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
   step = (p->walk_dir * p->walkspeed); 
  // if (p->scene->map[(int)p->y / 50][(int)p->x / 50] != '0' && p->walk_dir > 0 && p->turn_dir != 0)
  // {
  //   p->x = p->x + 1;
  //   p->y = p->y + 1;
  //   return (0);
  // }   
  p->rot_angle += (p->turn_dir * p->turnspeed);
  step = (p->walk_dir * p->walkspeed);
  x = p->x + 5 + cos(p->rot_angle) * (step);
  y = p->y + 5 + sin(p->rot_angle) * (step);
  if (p->scene->map[(int)(y - 5) / 50][(int)(x - 5) / 50] != '0')
  {
    x = p->x + 5;
    y = p->y + 5;
    return (0);
  }
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
      if (p->scene->map[j][i] != '0')
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

int ft_strlen2(char **map)
{
  int i;

  i = 0;
  while (map[i])
    i++;
  return (i) ; 
}
int main(int ac, char **av)
{
	void	*mlx_p;
	void	*mlx_win;
  t_mlx *p;
  if (ac != 2)
    return (0);
	mlx_p = mlx_init();
	if (!mlx_p)
		return (ft_error2(1));
  p = p_init();  
  p->scene = map_parse(av[1]);
  p->scene->map_w = ft_strlen(*p->scene->map);
  p->scene->map_h = ft_strlen2(p->scene->map);
  printf("%d\n%d\n", p->scene->map_w, p->scene->map_h);    
	mlx_win = mlx_new_window(mlx_p, p->scene->map_w * 50, p->scene->map_h * 50, "cub3d");
  p->mlx_p = mlx_p;
  p->mlx_win = mlx_win;
  if (!mlx_win)
		return (ft_error2(1));
  redraw(p);
  mlx_hook(p->mlx_win, 2, 0, key_hook, p);
  mlx_hook(p->mlx_win, 3, 0, key_hook2, p);
  mlx_hook(p->mlx_win, 6, 0, mouse_hook, p);
  mlx_loop_hook(p->mlx_p, step, (void *)p);
	mlx_loop(mlx_p);
}
