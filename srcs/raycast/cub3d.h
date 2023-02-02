/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:13:40 by idelfag           #+#    #+#             */
/*   Updated: 2023/01/30 22:59:41 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "get_next_line/get_next_line.h"
#include "libft/libft.h"

typedef struct scene
{
    char    **map;
    char    *north_tex;
    char    *south_tex;
    char    *west_tex;
    char    *east_tex;
    int     floor;
    int     ceiling;
} t_scene;