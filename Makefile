# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbopp <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/11 14:38:33 by lbopp             #+#    #+#              #
#    Updated: 2019/01/19 14:13:53 by lbopp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
CFLAGS = -Wall -Werror -Wextra -g
LIBFT = libft/
CC = clang
SRC_NAME = ft_malloc.c\
	   free.c\
	   calloc.c\
	   realloc.c\
	   show_alloc_mem.c\
		 check_data.c\
		 create_page.c\
	   align_data.c
SRC_PATH = src
SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(SRC:.c=.o)
RM = rm -f

.PHONY: all, clean, fclean, re

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJ) -I includes -I libft/includes -lft -L libft -g
	ln -s libft_malloc_$(HOSTTYPE).so libft_malloc.so

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I includes -I libft/includes

clean:
	make -C libft clean
	unlink libft_malloc.so
	$(RM) $(OBJ)

fclean: clean
	make -C libft fclean
	$(RM) $(NAME)

re: fclean all
