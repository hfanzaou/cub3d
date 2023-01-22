# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/16 04:48:24 by hfanzaou          #+#    #+#              #
#    Updated: 2023/01/18 10:14:06 by hfanzaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

SRCS = srcs/raycast/main.c

SFLAGS = -L /usr/local/lib/ -lmlx -framework OpenGL -framework AppKit -I /usr/locale/include

FLAGS = -Wall -Werror -Wextra

CC = gcc

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(SFLAGS) $(FLAGS) $(OBJS) -o $@

%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@
	
clean :
	rm $(OBJS)

fclean : clean
	rm $(NAME)

re : fclean all		