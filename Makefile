# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajana <ajana@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/16 04:48:24 by hfanzaou          #+#    #+#              #
#    Updated: 2023/02/19 06:07:46 by ajana            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

LIBFT = srcs/libft/libft.a

SRCS = srcs/raycast/main.c srcs/raycast/raycast.c srcs/raycast/utils.c srcs/raycast/raycast_utils.c srcs/raycast/big_draw.c srcs/raycast/mini_draw.c \
	srcs/raycast/init.c srcs/raycast/hooks.c srcs/get_next_line/get_next_line.c srcs/get_next_line/get_next_line_utils.c srcs/raycast/step.c \
	srcs/parsing/scene_init.c srcs/parsing/read_file.c srcs/parsing/elements.c srcs/parsing/parse_utils.c srcs/parsing/textures.c srcs/parsing/map_parse.c

SFLAGS = -L /usr/local/lib/ -lmlx -framework OpenGL -framework AppKit -I /usr/locale/include 

FLAGS = -Wall -Werror -Wextra -D BUFFER_SIZE=1

CC = gcc

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(LIBFT)
	$(CC) $(SFLAGS) $(FLAGS) $(OBJS) $(LIBFT) -o $@

$(LIBFT) : 
	make -C srcs/libft

%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -f $(OBJS)
	make -C srcs/libft clean

fclean : clean
	rm -f $(NAME)
	make fclean -C srcs/libft

re : fclean all		