/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 04:46:46 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/15 02:03:06 by ajana            ###   ########.fr       */
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
  // if (map->map[y / 50][x / 50] == '1')
  //   return (1);
  if (i == 1 && (map->map[(y + 1)/50][x/50] == '1'))
      return (1);
  else if (i == 0 && (map->map[y/50][(x + 1)/50] == '1'))    
      return (1);
  else if (i == 2)
  {
    if (map->map[y/50][(x - 1)/50] == '1' || map->map[y/50][(x + 1)/50] == '1')
      return (1);
    else if ((map->map[(y - 1)/50][x/50] == '1' || map->map[(y + 1)/50][x/50] == '1'))
      return (1);  
  }
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
  //printf("here\n");
  if (ray->hvy_inter > p->scene->map_h * p->tile_size || ray->hvy_inter < 0 || ray->hvx_inter > ft_strlen(p->scene->map[(int)ray->hvy_inter / 50]) * p->tile_size || ray->hvx_inter < 0)
      {
        cor->x = ray->hvx_inter;
        cor->y = ray->hvy_inter;
      return (cor); 
      }   
  while (p->scene->map[(int)(ray->hvy_inter/50)][(int)(ray->hvx_inter/50)] == '0' 
  && !check_collision(p->scene, (int)ray->hvx_inter, (int)ray->hvy_inter, f))
  {
    //put_dot(p, ray->hvx_inter, ray->hvy_inter);
    ray->hvy_inter += ray->dy;
    ray->hvx_inter += ray->dx;
   // printf("x = %F\ny = %f\n dx = %f\n dy = %f\n", ray->hvx_inter, ray->hvy_inter, ray->dx, ray->dy);
    if (ray->hvy_inter > p->scene->map_h * p->tile_size 
    || ray->hvy_inter < 0 || ray->hvx_inter > ft_strlen(p->scene->map[(int)ray->hvy_inter / 50]) * p->tile_size 
    || ray->hvx_inter < 0)
    {
       cor->x = ray->hvx_inter;
      cor->y = ray->hvy_inter;
      return (cor);
    }
    // if (p->scene->map[(int)ray->hvy_inter/p->tile_size][(int)ray->hvx_inter/p->tile_size] != '0')
    // {
    //   cor->x = ray->hvx_inter;
    //   cor->y = ray->hvy_inter;
    //   return (cor);
    // }
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

t_cor  *castone(t_mlx *p, t_ray *ray)
{
  float x;
  float y;
  t_cor *h;
  t_cor *v;
  int i;
  i = 0;
    x = p->x - cos(ray->ray) * i;
    y = p->y - sin(ray->ray) * i;
  while (p->scene->map[(int)y / p->tile_size][(int)x / p->tile_size] == '0' && !check_collision(p->scene, (int)x, (int)y, 1))
  {
    x = p->x - cos(ray->ray) * i;
    y = p->y - sin(ray->ray) * i;
   //mlx_pixel_put(p->mlx_p, p->mlx_win, x, y, 16711680);
   //mlx_pixel_put(p->mlx_p, p->mlx_win, x / 5, y / 5, 16711680);
    i += 4;
  }
 ray = ft_norm(ray);
  ray->dir = raydir(ray->ray);
  h = ft_hor(p, ray, 1);
  h->dis = sqrt((p->x - h->x) * (p->x - h->x) +  (p->y - h->y) * (p->y - h->y));
  v = ft_hor(p, ray, 0);
  v->dis = sqrt((p->x - v->x) * (p->x - v->x) +  (p->y - v->y) * (p->y - v->y));
  if (!v || !h)
    return NULL;
  if (h->dis > v->dis)
  {
    ray->distance = v->dis;
    //put_dot(p, v->x, v->y);
    return (v);
  }
  else 
  {
    ray->distance = h->dis; 
    //put_dot(p, h->x, h->y);
    return (h);  
  }
}
int draw_wall(t_mlx *p, t_ray *ray, t_cor *cor)
{
  float proj_plane;
  float wall_hight;
  int x;
  int y;
  int i;
  int j;
  int r;
  (void)cor;
  
  proj_plane = (1200 / 2) / tan(p->fov / 2);
  wall_hight = round((p->tile_size / ray->distance) * proj_plane);
  x = ray->index;
  i = 0;
  if (wall_hight >= 1200)
  {
    while (i < 1200)
    {
      p->xpm[i * 1200 + x] = 16711680;
      i++;
    }
    return 0;
  }
  if (x > 1200)
    return 0;
  //printf("index = %d\ndistance = %f\n", ray->index, ray->distance);
  y = (1200 / 2) - (wall_hight / 2);
  if (y > 1200)
    return 0;
  i = 0;
  //printf("gsdfg\n");
  j = 0;
  while (i < y)
  {
    p->xpm[j * 1200 + x] = 0x050A30;
    if (i % 100 == 0)
    {
      r = rand()%(y * 1000);
      if (r < y * 1200)
      p->xpm[r] = 0xFFFFFF;
    }
    //printf("j * 1200 + x = %d\n", j * 1200 + x);
    j++;
    i++;
  }
  while (y < ((1200 / 2) + (wall_hight / 2)))
  {
    p->xpm[j * 1200 + x] = 16711680;
    j++;
    y++;
  }
  while (y < 1200)
  {
    p->xpm[j * 1200 + x] = 8421504;
    j++;
    y++;
  }
  return (0);
}
void  draw_center(t_mlx *p)
{
  int i;
  int j;
  i = 0;
  j = 1200 * 550 + 595;
  i = 595;
  while (i < 605)
  {
    i = 595;
    while (j < 1200 * 650 + 605)
    {
      p->xpm[j] = 8421504;
      j++;
    }
    i++;
  }
}
t_ray *ft_raycast(t_mlx *p)
{
  float i;
  t_cor *cor;
  
  p->fov = 60 * (M_PI / 180);
  t_ray *ray;
  ray = malloc(sizeof(t_ray));
  i = 0;
  ray->ray = p->rot_angle - (p->fov / 2);
  ray->index = 0;
  while (i < 1199)
  {
    cor = castone(p, ray);
    ray->ray += ((p->fov) / (1200));
    ray->index = i;
    draw_wall(p, ray, cor);
    //printf("ray = %f\n", ray->ray);
    i++;
  }
  //draw_center(p);
  return (ray);
}

void  redraw(t_mlx *p)
{
  int i;
  int j;
  j = 0;
  while (p->scene->map[j])
  {
    i = 0;
    while (p->scene->map[j][i])
    {
      if (ft_strchr("NSEW", p->scene->map[j][i]))
      {
        p->x = p->tile_size * i + p->tile_size / 2;
        p->y = p->tile_size * j + p->tile_size / 2;
        p->scene->map[j][i] = '0';
      }    
      i++;
    }
    j++;
  }
}

t_mlx *p_init(char *path)
{
    t_mlx *p;
    p = malloc(sizeof(t_mlx));
	
    ft_memset(p, 0, sizeof(t_mlx));
	p->scene = map_parse(path);
	if (!p->scene)
		return (NULL);
    p->rot_angle = M_PI / 2;
    p->turnspeed = 2 * (M_PI / 180);
    p->walkspeed = 3;
    p->slide_angle = M_PI / 2;
    p->tile_size = 50;
    p->bpp = 1;
    return (p);
}

void fill(t_mlx *p, int x, int y, int color)
{
  int i;
  int j;
  i = 0;
  while (i < 100)
  {
    j = 0;
    while (j < 100)
    {
      p->xpm[(y + i) * 1200 + x + j] = color;
      j++;
    }
    i++;
  }
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
        //fill(p, x, y, 16777215);
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
  //printf("x = %f\ny = %f\n", x, y);
  if (check_collision(p->scene, (int)x, (int)y, 2))
    return (0);
  p->x = x;
  p->y = y;
  mlx_clear_window(p->mlx_p, p->mlx_win);
  //printf("here\n");
  redraw(p);
  ft_raycast(p);
  mlx_put_image_to_window(p->mlx_p, p->mlx_win, p->img, 0, 0);
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
  	t_mlx	*p;

  	if (ac != 2)
  	  return (0);
  	p = p_init(av[1]);
  	if (!p)
		return (1);
  	p->mlx_p = mlx_init();
	p->mlx_win = mlx_new_window(p->mlx_p, 1200, 1200, "cub3d");
  	if (!(p->mlx_p) || !(p->mlx_win))
		return (ft_error("Error initializing mlx\n"));
  	p->img = mlx_new_image(p->mlx_p, 1200, 1200);
  	p->xpm = (int *)mlx_get_data_addr(p->img, &p->bpp, &p->size_line, &p->endian);
  	redraw(p);
  	//draw_mini(p);
  	mlx_hook(p->mlx_win, 2, 0, key_hook, p);
  	mlx_hook(p->mlx_win, 3, 0, key_hook2, p);
  	mlx_hook(p->mlx_win, 6, 0, mouse_hook, p);
  	mlx_loop_hook(p->mlx_p, step, (void *)p);
	mlx_loop(p->mlx_p);
}
