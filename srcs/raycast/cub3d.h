/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:13:40 by idelfag           #+#    #+#             */
/*   Updated: 2023/02/17 06:29:58 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "mlx.h"
#include "get_next_line/get_next_line.h"
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
int		ft_error(char *err_msg);
int		strlen2(char **ptr);
int get_texture(t_cor *inter, t_ray *ray);
int textures_init(t_mlx *p);