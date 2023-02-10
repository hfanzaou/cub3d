/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 04:46:46 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/10 19:08:15 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

typedef struct s_mlx
{
  void    *mlx_p;
  void    *mlx_win;
  float   x;
  float   y;
  int     turn_dir;
  int     walk_dir;
  float   rot_angle;
  int     slide_dir;
  float   slide_angle;
  float   walkspeed;
  float   turnspeed;
  float   fov;
  int     tile_size;
  int    *xpm;
  int     bpp;
  t_scene *scene;
}         t_mlx;

typedef struct s_ray
{
  float ray;
  float hvx_inter;
  float hvy_inter;
  float dx;
  float dy;
  int dir;
  float distance;
  int f;
  int index;
} t_ray;

typedef struct s_cor
{
  int x;
  int y;
  float dis;
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
  return (1);   
}

void  intersec(t_mlx *p, t_ray *ray)
{
  int inter_dir;
  //printf("%f\n", ray->ray);
  inter_dir = ft_dir(ray->dir, 1);
  if (inter_dir == -1)
  {
    ray->hvy_inter = floor((p->y) / p->tile_size) * p->tile_size - 1;
    ray->hvx_inter = p->x + ((ray->hvy_inter - p->y)/tan(ray->ray));
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
  //printf("%f\n", ray->ray);
  if (inter_dir == -1)
  {
    ray->hvx_inter = floor((p->x) / p->tile_size) * p->tile_size - 1;
    ray->hvy_inter = p->y - ((p->x - ray->hvx_inter) * tan(ray->ray));
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

int ft_dir2(int dir, int f)
{
  if (f == 1 && (dir == 1 || dir == 4))
    return (1);
  if (f == 0 && (dir == 1 || dir == 2))
    return (1);
  return (-1);    
}
int check_collision(t_scene *map, int x, int y, int i)
{
  if (map->map[y / 50][x / 50] == '0')
    return (0);
  if (map->map[y / 50][(x - i) / 50] != '0')
    return (1);
  if (map->map[(y - i) / 50][x / 50] != '0')
    return (1);
  if (map->map[y / 50][(x + i) / 50] != '0')
    return (1);
  if (map->map[(y + i)/ 50][x / 50] != '0')
    return (1);
  return (0);        
}
t_cor *ft_hor(t_mlx *p, t_ray *ray, int f)
{
  int h;
  int w;
  t_cor *cor;
  cor = malloc(sizeof(t_cor));
  w = 10;
  h = 10;
  
  //printf("whface = %d\n", ray->dir);
  if (f == 1)
    intersec(p, ray);
  else 
    intersec2(p, ray);  
  //printf("px = %f\npy = %f\nhx = %f\nhy = %f\ntx = %f\nty = %f\n", p->x, p->y, ray->hvx_inter, ray->hvy_inter, ray->dx, ray->dy);
  if (ray->hvy_inter > p->scene->map_h * p->tile_size || ray->hvy_inter < 0 || ray->hvx_inter > p->scene->map_w * p->tile_size || ray->hvx_inter < 0)
      {
        cor->x = ray->hvx_inter;
        cor->y = ray->hvy_inter;
      return (cor); 
      }   
  while (p->scene->map[(int)ray->hvy_inter/p->tile_size][(int)ray->hvx_inter/p->tile_size] == '0' 
  && !check_collision(p->scene, (int)ray->hvx_inter, (int)ray->hvy_inter, 1))
  {
    //put_dot(p, ray->hvx_inter, ray->hvy_inter);
    ray->hvy_inter += ray->dy;
    ray->hvx_inter += ray->dx;
    if (ray->hvy_inter > p->scene->map_h * p->tile_size || ray->hvy_inter < 0 || ray->hvx_inter > p->scene->map_w * p->tile_size || ray->hvx_inter < 0)
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
  //int hypotv = -1;
  //float distance;
  t_cor *h;
  t_cor *v;
  // float ay;
  // float ax;
  int i;
  i = 0;
    x = p->x - cos(ray->ray) * i;
    y = p->y - sin(ray->ray) * i;
  // else
  // {
  //   x = p->x + 6 - cos(ray->ray) * i;
  //   y = p->x + 6 - sin(ray->ray) * i;
  // }
  while (p->scene->map[(int)y / p->tile_size][(int)x / p->tile_size] == '0')
  {
    x = p->x - cos(ray->ray) * i;
    y = p->y - sin(ray->ray) * i;
   //mlx_pixel_put(p->mlx_p, p->mlx_win, x, y, 16711680);
   mlx_pixel_put(p->mlx_p, p->mlx_win, x / 5, y / 5, 16711680);
    i += 4;
  }
  // if (p->scene->map[(int)p->x / 50][(int)p->y / 50] == '1')
  // {  
  //   p->x = p->x;
  //   p->y = p->y;
  // }
  //distance = 0;
 ray = ft_norm(ray);
 //printf("ray = %f\n", ray->ray);
 //printf("ray = %f\n %f\n", ray->ray);
  ray->dir = raydir(ray->ray);
  h = ft_hor(p, ray, 1);
  h->dis = sqrt((p->x - h->x) * (p->x - h->x) +  (p->y - h->y) * (p->y - h->y));
  v = ft_hor(p, ray, 0);
  v->dis = sqrt((p->x - v->x) * (p->x - v->x) +  (p->y - v->y) * (p->y - v->y));
  if (!v || !h)
    return ;
  // printf("hypotv = %f\n", hypot(v->x, v->y));
  // printf("hypoth = %f\n", hypot(h->x, h->y));  
  // if (ray->dir == 1)  
  //   hypotv = h->x >= v->x && h->y >= v->y;
  // else if (ray->dir == 2)
  //   hypotv = v->x >= h->x && h->y >= v->y;
  // else if (ray->dir == 3)
  //   hypotv = v->x >= h->x && v->y >= h->y;
  // else if (ray->dir == 4)
  //   hypotv =  v->x <= h->x && v->y >= h->y; 
  // if (hypotv)
  // {
  //   distance = sqrt((p->x - h->x) * (p->x - h->x) +  (p->y - h->y) * (p->y - h->y));
  //   //put_dot(p, h->x, h->y);
  // }
  // else
  // {
  //   distance = sqrt((p->x - v->x) * (p->x - v->x) +  (p->y - v->y) * (p->y - v->y));
  //   //put_dot(p, v->x, v->y);
  // }
  if (h->dis > v->dis)
    ray->distance = v->dis;
  else 
    ray->distance = h->dis;   
}
int draw_wall(t_mlx *p, t_ray *ray)
{
  float proj_plane;
  float wall_hight;
  int x;
  int y;
  int i;
  int j;
  //void *img;
  proj_plane = (1200 / 2) / tan(p->fov / 2);
  wall_hight = (p->tile_size / ray->distance) * proj_plane;
  x = ray->index;
  //printf("index = %d\ndistance = %f\n", ray->index, ray->distance);
  y = (1200 / 2) - (wall_hight / 2);
  i = 0;
  //printf("gsdfg\n");
  p->bpp = 1; 
 // p->xpm = (int *)mlx_get_data_addr(p->mlx_p, &p->bpp, (int *)1200, (int *)1200);
  j = 0;
  while (i < y)
  {
    mlx_pixel_put(p->mlx_p, p->mlx_win, x * 4, i, 8308963);
    // p->xpm[j + i] = (char *)8308963;
    // j += 1200;
    i++;
  }
  while (y < ((1200 / 2) + (wall_hight / 2)))
  {
    mlx_pixel_put(p->mlx_p, p->mlx_win, x * 4, y, 16711680);
    // p->xpm[j + y] = (char *)16711680;
    // j += 1200;
    y++;
  }
  while (y < 1200)
  {
    mlx_pixel_put(p->mlx_p, p->mlx_win, x * 4, y, 8421504);
    // p->xpm[j + y] = (char *)8421504;
    // j += 1200;
    y++;
  }
  // img = mlx_xpm_to_image(p->mlx_p, p->xpm, (int *)1200, (int *)1200);
  // mlx_put_image_to_window(p->mlx_p, p->mlx_win, img, 1200, 1200);
  return (0);
}
t_ray *ft_raycast(t_mlx *p)
{
  float i;

  p->fov = 60 * (M_PI / 180);
  t_ray *ray;
  ray = malloc(sizeof(t_ray));
  i = 0;
  ray->ray = p->rot_angle - (p->fov / 2);
  ray->index = 0;
  while (i < 1200 / 4)
  {
    castone(p, ray);
    ray->ray += p->fov / (1200 / 4);
    ray->index = i;
    draw_wall(p, ray);
    //printf("ray = %f\n", ray->ray);
    i++;
  }
  return (ray);
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
  img_ptr = mlx_xpm_file_to_image(p->mlx_p, "ground.xpm", &w, &h);
  img_ptr2 = mlx_xpm_file_to_image(p->mlx_p, "wall.xpm", &w, &h);
  img_ptr3 = mlx_xpm_file_to_image(p->mlx_p, "p.xpm", &w, &h);
  while (p->scene->map[j])
  {
    i = 0;
    h = j * p->tile_size;
    while (p->scene->map[j][i])
    {
      w = i * p->tile_size;
      // if (p->scene->map[j][i] == '1')
      //   mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr, i * p->tile_size, j * p->tile_size);
      // else if (p->scene->map[j][i] == '0')
      //   mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr2, i * p->tile_size, j * p->tile_size);
      if (ft_strchr("NSEW", p->scene->map[j][i]))
      {
        p->x = p->tile_size * i + p->tile_size / 2;
        p->y = p->tile_size * j + p->tile_size / 2;
       // printf("p->x = %f\n", p->x);
        // mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr2, i * p->tile_size, j * p->tile_size);
        // mlx_put_image_to_window(p->mlx_p, p->mlx_win, img_ptr3, p->x, p->y);
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
  // while (k <= 100)
  // {
  //   mlx_pixel_put(p->mlx_p, p->mlx_win, p->x + 5 - cos(p->rot_angle) * k, p->y + 5 - sin(p->rot_angle) * k, 16711680);
  //   k++;
  // }
}

t_mlx *p_init()
{
    t_mlx *p;
 
    p = malloc(sizeof(t_mlx));
    p->x = 0;
    p->y = 0;
    p->walk_dir = 0;
    p->turn_dir = 0;
    p->slide_dir = 0;
    p->rot_angle = M_PI / 2;
    p->turnspeed = 5 * (M_PI / 180);
    p->walkspeed = 5;
    p->slide_angle = M_PI / 2;
    p->tile_size = 50;
    return (p);
}

int draw_mini(t_mlx *p)
{
  char **map;
  int j;
  int i;
  void *img1;
  void *img2;
  void *img3;
  int h;
  int w;
  int x;
  int y;
 img1 = mlx_xpm_file_to_image(p->mlx_p, "ground.xpm", &w, &h);
  img2 = mlx_xpm_file_to_image(p->mlx_p, "wall.xpm", &w, &h);
  img3 = mlx_xpm_file_to_image(p->mlx_p, "p.xpm", &w, &h);
  
  j = 0;
  map = p->scene->map;
  while (map[j])
  {
    i = 0;
    while (map[j][i])
    {
      if (p->scene->map[j][i] == '1')
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img1, i * p->tile_size / 5, j * p->tile_size / 5);
      else if (p->scene->map[j][i] == '0')
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img2, i * p->tile_size / 5, j * p->tile_size / 5);
      if (ft_strchr("NSEW", p->scene->map[j][i]))
      {
        x = (p->tile_size / 5) * i + p->tile_size / 10;
        y = (p->tile_size / 5) * j + p->tile_size / 10;
       //printf("p->x = %f\n", p->x);
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img2, i * p->tile_size / 5, j * p->tile_size / 5);
        mlx_put_image_to_window(p->mlx_p, p->mlx_win, img3, x, y);
        p->scene->map[j][i] = '0';
      } 
      i++;
    }
    j++;
  }
  mlx_put_image_to_window(p->mlx_p, p->mlx_win, img3, p->x / 5, p->y / 5);
  mlx_destroy_image(p->mlx_p, img1);
  mlx_destroy_image(p->mlx_p, img2);
  mlx_destroy_image(p->mlx_p, img3);
  return (0);
}
int  step(void *ptr)
{

  t_mlx *p;
  p = (t_mlx *)ptr;
  float step;
  float x;
  float y;
  float slide;
  if (p->walk_dir == 0 && p->turn_dir == 0 && p->slide_dir == 0)
    return 0;
  step = (p->walk_dir * p->walkspeed);
  slide = (p->slide_dir * p->walkspeed);
  // if (p->scene->map[(int)p->y / 50][(int)p->x / 50] != '0' && p->walk_dir > 0 && p->turn_dir != 0)
  // {
  //   p->x = p->x + 1;
  //   p->y = p->y + 1;
  //   return (0);
  // }   
  p->rot_angle += (p->turn_dir * p->turnspeed);
  p->slide_angle = p->rot_angle - (M_PI / 2);
  //printf("rot_angle %f\nslide_angle = %f\nslide = %f\n",p->rot_angle, p->slide_angle, slide);
  x = p->x + cos(p->rot_angle) * (step) + cos(p->slide_angle) * slide;
  y = p->y + sin(p->rot_angle) * (step) + sin(p->slide_angle) * slide;
  printf("x = %f\ny = %f\n", x, y);
  if (check_collision(p->scene, (int)x, (int)y, 1))
    return (0);
  p->x = x;
  p->y = y;
  mlx_clear_window(p->mlx_p, p->mlx_win);
  printf("here\n");
  redraw(p);
  ft_raycast(p);
  draw_mini(p);
  return (0);
}

int key_hook(int key, t_mlx *p)
{
  if ((key == 126 || key == 13))
    p->walk_dir = -1;
  if (key == 125 || key == 1)
    p->walk_dir = 1;
  if (key == 123)
    p->turn_dir = -1;
  if (key == 124)
    p->turn_dir = 1;
  if (key == 2)
    p->slide_dir = 1;
  if (key == 0)
    p->slide_dir = -1;    
  return (0); 
}

int   key_hook2(int key, t_mlx *p)
{
   if (key == 126 || key == 13)
    p->walk_dir = 0;
 if (key == 125 || key == 1)
    p->walk_dir = 0;  
 if (key == 123)
    p->turn_dir = 0;
  if (key == 124)
    p->turn_dir = 0;
  if (key == 2)
    p->slide_dir = 0;
  if (key == 0)
    p->slide_dir = 0; 
  return (0);
}

int mouse_hook(int x, int y, t_mlx *p)
{
  (void)y;
  if (x > 1200 / 2 + 400)
    p->turn_dir = 1;
  else if (x < 1200 / 2 - 400)
    p->turn_dir = -1;
  else
    p->turn_dir = 0;   
  return (0);    
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
  if (!p->scene)
    return (1);
 // printf("%d\n%d\n", p->scene->map_w, p->scene->map_h);    
	mlx_win = mlx_new_window(mlx_p, 1200, 1200, "cub3d");
  p->mlx_p = mlx_p;
  p->mlx_win = mlx_win;
  if (!mlx_win)
		return (ft_error2(1));
  redraw(p);
  draw_mini(p);
  mlx_hook(p->mlx_win, 2, 0, key_hook, p);
  mlx_hook(p->mlx_win, 3, 0, key_hook2, p);
  mlx_hook(p->mlx_win, 6, 0, mouse_hook, p);
  mlx_loop_hook(p->mlx_p, step, (void *)p);
	mlx_loop(mlx_p);
}
