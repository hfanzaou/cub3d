/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 23:19:27 by hfanzaou          #+#    #+#             */
/*   Updated: 2023/02/17 23:20:10 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
