# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/24 13:52:38 by ilbonnev          #+#    #+#              #
#    Updated: 2025/04/06 23:32:13 by ilbonnev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = exe
CC = gcc
CFLAGS = -g -Wall -Werror -Wextra -fsanitize=thread

SRC = $(wildcard philo/*.c) 

OBJ = $(SRC:%.c=obj/%.o)

LIBFT_PATH = libft
LIBFT_FILE = $(LIBFT_PATH)/libft.a

all: $(NAME)

$(NAME): $(LIBFT_FILE) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_FILE) -o $(NAME)

$(LIBFT_FILE):
	make -C $(LIBFT_PATH)

clean:
	rm -rf ./obj
	make clean -C $(LIBFT_PATH)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_PATH)

re: fclean all

obj/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re
