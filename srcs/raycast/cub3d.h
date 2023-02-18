/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:13:40 by idelfag           #+#    #+#             */
/*   Updated: 2023/02/18 22:02:22 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "mlx.h"
#include "libft/libft.h"

enum tex_id
{
  north,
  south,
  east,
  west,
};

typedef struct  s_tex
{
  void  *tex_p;
  char  *data;
  int   bpp;
  int   size_line;
  int   endian;
  int   hight;
  int   width;
} t_tex;

typedef struct scene
{
    char    **map;
    char    *north_tex;
    char    *south_tex;
    char    *west_tex;
    char    *east_tex;
    int     floor;
    int     ceiling;
    int     map_w;
    int     map_h;
    int     player_dir;
} t_scene;

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
  char    *xpm;
  int     bpp;
  void   *img;
   int size_line;
  int endian;
  int close;
  t_tex *textures;
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
  float x;
  float y;
  float dis;
  int f;
}  t_cor;

t_scene *map_parse(char *path);
int		  ft_error(char *err_msg);
int		  strlen2(char **ptr);
int     get_texture(t_cor *inter, t_ray *ray);
int     textures_init(t_mlx *p);
int     elements_check(char ***file, t_scene *scene);
int     read_file(char *path, char **file, int lines_count);
void    ft_free(char **ptr);
int     map_check(char **file, t_scene *scene);
int	    is_space(char c);
int     got_all_elements(t_scene *scene);
int     is_closed(char *line, char *up_line, char *down_line, int ind);
char	  *get_next_line(int fd);
int     lines_count(char *path);



//===================HOOKS==================//
int key_hook(int key, t_mlx *p);
int key_hook2(int key, t_mlx *p);
int mouse_hook(int x, int y, t_mlx *p);


//===============INITIALIZE==================//
t_mlx *p_init(char *path);
int step_init(t_mlx *p);


//============drawing========//
int draw_mini(t_mlx *p);
int draw_wall(t_mlx *p, t_ray *ray, t_cor *cor);
int check_collision(t_scene *map, int x, int y, int i);
int get_x(t_mlx *p, t_cor *cor, float wall_hight, t_ray *ray);
int get_color(t_tex *img, int x, int y);


//============RAYCASTING=========//
void  ft_raycast(t_mlx *p);
int  step(void *ptr);
t_ray *ft_norm(t_ray *ray);
int ft_dir(int dir, int f);
int raydir(float ray);
void  intersec(t_mlx *p, t_ray *ray);
void  intersec2(t_mlx *p, t_ray *ray);
t_cor *put_cor(float x, float y, t_cor *cor);
int beyond_map(float ray_x, float ray_y, t_mlx *p);