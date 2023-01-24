/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 04:46:46 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/01/24 19:02:22 by hfanzaou         ###   ########.fr       */
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

// void  ft_ver()
// {
  
// }

void  castone(t_mlx *p, float ray)
{
  float x;
  float y;
  // float ay;
  // float ax;
  int i;
  i = 1;
  x = p->x + 5 - cos(ray) * i;
  y = p->y + 5 - sin(ray) * i;
  while (worldMap[(int)y / 50][(int)x / 50] == 0)
  {
    x = p->x + 5 - cos(ray) * i;
    y = p->y + 5 - sin(ray) * i;
    mlx_pixel_put(p->mlx_p, p->mlx_win, x, y, 16711680);
    //ft_ver();
    i += 3;
  }
}

int ft_raycast(t_mlx *p)
{
  float fov = 60 * (M_PI / 180);
  float i;
  float rangle;
  i = 0;
  rangle = p->rot_angle - (fov / 2);
  while (i < 1200 / 4)
  {
    castone(p, rangle);
    rangle += fov / (1200 / 4);
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
  redraw(p);
  mlx_hook(p->mlx_win, 2, 0, key_hook, p);
  mlx_hook(p->mlx_win, 3, 0, key_hook2, p);
  //mlx_hook(p->mlx_win, 6, 0, mouse_hook, p);
  mlx_loop_hook(p->mlx_p, step, (void *)p);
	mlx_loop(mlx_p);
}
