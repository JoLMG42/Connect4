# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mle-faou <mle-faou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/11 01:35:21 by mle-faou          #+#    #+#              #
#    Updated: 2022/06/12 16:05:03 by mle-faou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = connect4
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3

MLX_PATH = mlx
MLX = ${MLX_PATH}/libmlx.a
MLX_FLAGS = -L. -lXext -L. -lX11

SRCS = main.c graphic.c
OBJS = $(SRCS:.c=.o)

MLX_PATH = ./mlx/
MLX = ${MLX_PATH}libmlx.a
MLX_FLAGS = -L. -lXext -L. -lX11

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) ${MLX}
	$(CC) $(OBJS) ${MLX} ${MLX_FLAGS} -o $@
	
all: $(NAME)

${MLX}:
	make -C ${MLX_PATH} do_configure > osef
	make -C ${MLX_PATH} > osef

clean:
	make -C ${MLX_PATH} clean 
	rm -f $(OBJS)

fclean: clean
	rm -rf mlx/obj
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
