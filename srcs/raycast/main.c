/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 04:46:46 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/17 06:14:18 by hfanzaou         ###   ########.fr       */
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

// void put_dot(t_mlx *p, int x, int y)
// {
//   int i;
//   int j;
//   i = 0;
//   j = 0;
//   while (i < 6)
//   {
//     j = 0;
//     while (j < 6)
//     {
//       mlx_pixel_put(p->mlx_p, p->mlx_win, x + i, y + j, 16711680);
//       j++;
//     }
//     i++;
//   }
// }

int check_collision(t_scene *map, int x, int y, int i)
{
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

int draw_ray(t_mlx *p, float angle)
{
  int x;
  int y;
  int i;
  i = 0;
    x = p->x - cos(angle) * i;
    y = p->y - sin(angle) * i;
    i = 0;
    while (p->scene->map[(int)y / p->tile_size][(int)x / p->tile_size] == '0')
    {
     x = p->x - cos(angle) * i;
    y = p->y - sin(angle) * i;
    ((unsigned int *)p->xpm)[(x / 5) * p->size_line + (y / 5) * (p->bpp / 8)] = 16711680;
   // mlx_pixel_put(p->mlx_p, p->mlx_win, x/5, y/5, 16711680);
    i++;
    }
  return (0);
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
    //put_dot(p, v->x, v->y);
    return (v);
  }
  else 
  {
    ray->distance = h->dis; 
    h->f = 1;
    //put_dot(p, h->x, h->y);
    return (h);  
  }
}
int get_color(t_imgs *img, int x, int y)
{
  char *color;
  //printf("x = %d\ny = %d\nw = %d\nh = %d\n", x, y, img->w, img->h);
  if (x >= img->w)
   x = img->w - 1;
  if (y >= img->h)
  y = img->w - 1; 
  color = &img->data[y * img->size_line1 + x * (img->bpp1/8)];
  return (*(unsigned int *)color);
}
int draw_ceil_floor(t_mlx *p, int x)
{
  int i;
  i = 0;
  while (i < 1200)
  {
    if (i < 1200 / 2)
      ((unsigned int *)p->xpm)[i * 1200 + x] = 0x050A30;
    else 
       ((unsigned int *)p->xpm)[i * 1200 + x] = 0x6F7378;
    i++;   
  }
  return (0);
}
int get_x(t_mlx *p, t_cor *cor, float wall_hight)
{
  int x;
  float pos;
  if (cor->f == 0)
  {
    p->imgs->data = mlx_get_data_addr(p->imgs->west, &p->imgs->bpp1, &p->imgs->size_line1, &p->imgs->endian1); 
    pos = cor->y - floor(cor->y / p->tile_size) * p->tile_size;
  }
  else
  {
    p->imgs->data = mlx_get_data_addr(p->imgs->south, &p->imgs->bpp1, &p->imgs->size_line1, &p->imgs->endian1);
    pos = cor->x - floor(cor->x / p->tile_size) * p->tile_size;
  }
  x = wall_hight * pos / p->tile_size;
  x = x / wall_hight * p->imgs->w;
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
  proj_plane = (1200 / 2) / tan(p->fov / 2);
  wall_hight = p->tile_size / (ray->distance * cos(ray->ray - p->rot_angle)) * proj_plane;
  win_cor.x = ray->index;
  hight = wall_hight;
  
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
  tex_cor.x = get_x(p, cor, wall_hight);
  draw_ceil_floor(p, win_cor.x);
  while (win_cor.y < ((1200/ 2) + (hight / 2)))
  {
    tex_cor.y = (win_cor.y - y2) / wall_hight * p->imgs->h;
    ((unsigned int *)p->xpm)[(int)win_cor.y * 1200 + (int)win_cor.x] = get_color(p->imgs, floor(tex_cor.x), floor(tex_cor.y));
    win_cor.y++;
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
     ((unsigned int *)p->xpm)[j] = 8421504;
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
  while (i < 1200)
  {
    cor = castone(p, ray);
    ray->ray += ((p->fov) / (1200));
    ray->index = i;
    draw_wall(p, ray, cor);
    //draw_ray(p, ray->ray);
    i++;
  }
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
    p->turnspeed = 5 * (M_PI / 180);
    p->walkspeed = 7;
    p->slide_angle = M_PI / 2;
    p->tile_size = 50;
    p->bpp = 1;
    return (p);
}

void fill_square(t_mlx *p, int x, int y, int color)
{
  int i;
  int j;
  i = 0;
  while (i < 10)
  {
    j = 0;
    while (j < 10)
    {
      if (i == 0 || j == 0 || i == 9 || j == 9)
        ((unsigned int *)p->xpm)[(y / 5 + i) * 1200 + x / 5 + j] = 0x000000;
      else  
        ((unsigned int *)p->xpm)[(y / 5 + i) * 1200 + x / 5 + j] = color;
      j += 2;
    }
    i += 2;
  }
}
void  fill_player(t_mlx *p, int x, int y, int color)
{
  int i;
  int j;
  
  i = -2;
  j = -2;
  while (i < 2)
  {
    j = -2;
    while (j < 2)
    {
      ((unsigned int *)p->xpm)[(y / 5 + i) * 1200 + x/5 + j] = color;
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
  int x;
  int y;
  
  j = 0;
  map = p->scene->map;
  while (map[j])
  {
    i = 0;
    while (map[j][i])
    {
      if (p->scene->map[j][i] == '1')
        fill_square(p, i * p->tile_size, j * p->tile_size, 0xFFFFFF);
        // mlx_put_image_to_window(p->mlx_p, p->mlx_win, p->imgset->img1, i * p->tile_size / 5, j * p->tile_size / 5);
      else if (p->scene->map[j][i] == '0')
        fill_square(p, i * p->tile_size, j * p->tile_size, 0x808080);
        // mlx_put_image_to_window(p->mlx_p, p->mlx_win, p->imgset->img2, i * p->tile_size / 5, j * p->tile_size / 5);
      if (ft_strchr("NSEW", p->scene->map[j][i]))
      {
        x = (p->tile_size / 5) * i + p->tile_size / 10;
        y = (p->tile_size / 5) * j + p->tile_size / 10;
        fill_square(p, i * p->tile_size, j * p->tile_size, 0x808080);
        fill_player(p, i * p->tile_size, j * p->tile_size, 0xFF0000);
        // mlx_put_image_to_window(p->mlx_p, p->mlx_win, p->imgset->img2, i * p->tile_size / 5, j * p->tile_size / 5);
        // mlx_put_image_to_window(p->mlx_p, p->mlx_win, p->imgset->img3, x, y);
        p->scene->map[j][i] = '0';
      } 
      i++;
    }
    j++;
  }
  fill_player(p, p->x, p->y, 0xFF0000);
  // mlx_destroy_image(p->mlx_p, img1);
  // mlx_destroy_image(p->mlx_p, img2);
  // mlx_destroy_image(p->mlx_p, img3);
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
  p->rot_angle += (p->turn_dir * p->turnspeed);
  p->slide_angle = p->rot_angle - (M_PI / 2);
  x = p->x + cos(p->rot_angle) * (step) + cos(p->slide_angle) * slide;
  y = p->y + sin(p->rot_angle) * (step) + sin(p->slide_angle) * slide;
  if (check_collision(p->scene, (int)x, (int)y, 2))
  {
    p->rot_angle -= (p->turn_dir * p->turnspeed);
    return (0);
  }
  p->x = x;
  p->y = y;
  mlx_clear_window(p->mlx_p, p->mlx_win);
  redraw(p);
  ft_raycast(p);
  draw_mini(p);
  mlx_put_image_to_window(p->mlx_p, p->mlx_win, p->img, 0, 0);
  return (0);
}
int step2(t_mlx *p)
{
   float step;
  float x;
  float y;
  float slide;
    step = (p->walk_dir * p->walkspeed);
  slide = (p->slide_dir * p->walkspeed);
  p->rot_angle += (p->turn_dir * p->turnspeed);
  p->slide_angle = p->rot_angle - (M_PI / 2);
  x = p->x + cos(p->rot_angle) * (step) + cos(p->slide_angle) * slide;
  y = p->y + sin(p->rot_angle) * (step) + sin(p->slide_angle) * slide;
  if (check_collision(p->scene, (int)x, (int)y, 2))
  {
    p->rot_angle -= (p->turn_dir * p->turnspeed);
    return (0);
  }
  p->x = x;
  p->y = y;
  mlx_clear_window(p->mlx_p, p->mlx_win);
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
    int h;
    int w;
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
  	p->xpm = mlx_get_data_addr(p->img, &p->bpp, &p->size_line, &p->endian);
    p->imgs = malloc(sizeof(t_imgs));
    p->imgs->north = mlx_xpm_file_to_image(p->mlx_p, "wall2.xpm", &p->imgs->w, &p->imgs->h);
    p->imgs->south = mlx_xpm_file_to_image(p->mlx_p, "wall1.xpm", &p->imgs->w, &p->imgs->h);
    p->imgs->east = mlx_xpm_file_to_image(p->mlx_p, "wall3.xpm", &p->imgs->w, &p->imgs->h);
    p->imgs->west = mlx_xpm_file_to_image(p->mlx_p, "wall4.xpm", &p->imgs->w, &p->imgs->h);
    p->imgset = malloc(sizeof(t_imgset)); 
    p->imgset->img1 = mlx_xpm_file_to_image(p->mlx_p, "ground.xpm", &w, &h);
    p->imgset->img2 = mlx_xpm_file_to_image(p->mlx_p, "wall.xpm", &w, &h);
    p->imgset->img3 = mlx_xpm_file_to_image(p->mlx_p, "p.xpm", &w, &h);
  	step2(p);
  	mlx_hook(p->mlx_win, 2, 0, key_hook, p);
  	mlx_hook(p->mlx_win, 3, 0, key_hook2, p);
  	mlx_hook(p->mlx_win, 6, 0, mouse_hook, p);
  	mlx_loop_hook(p->mlx_p, step, (void *)p);
	  mlx_loop(p->mlx_p);
}
