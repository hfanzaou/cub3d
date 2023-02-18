# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajana <ajana@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/16 04:48:24 by hfanzaou          #+#    #+#              #
#    Updated: 2023/02/17 23:59:21 by ajana            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

LIBFT = libft.a

SRCS = srcs/raycast/main.c srcs/raycast/init.c  srcs/raycast/read_file.c  srcs/raycast/elements.c srcs/raycast/hooks.c srcs/raycast/map_parse.c srcs/raycast/textures.c srcs/raycast/get_next_line/get_next_line.c srcs/raycast/get_next_line/get_next_line_utils.c

SFLAGS = -L /usr/local/lib/ -lmlx -framework OpenGL -framework AppKit -I /usr/locale/include 

FLAGS = -Wall -Werror -Wextra -D BUFFER_SIZE=1 -fsanitize=address

CC = gcc

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(LIBFT)
	$(CC) $(SFLAGS) $(FLAGS) $(OBJS) srcs/raycast/libft/$(LIBFT) -o $@

$(LIBFT) : 
	make -C srcs/raycast/libft 

%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@
	
clean :
	rm -f $(OBJS)

fclean : clean
	make fclean -C srcs/raycast/libft
	rm -f $(NAME)

re : fclean all		