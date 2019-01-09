# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbopp <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/11 14:38:33 by lbopp             #+#    #+#              #
#    Updated: 2019/01/09 17:38:20 by lbopp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = malloc_test.so
CFLAGS = -Wall -Werror -Wextra -g
LIBFT = libft/
CC = clang
SRC_NAME = ft_malloc.c
SRC_PATH = src
SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(SRC:.c=.o)
RM = rm -f

.PHONY: all, clean, fclean, re

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJ) -I includes -I libft/includes -lft -L libft -g -O

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I includes -I libft/includes -O

clean:
	make -C libft clean
	$(RM) $(OBJ)

fclean: clean
	make -C libft fclean
	$(RM) $(NAME)

re: fclean all
