# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbopp <lbopp@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/02 11:07:10 by lbopp             #+#    #+#              #
#    Updated: 2017/03/01 13:43:56 by lbopp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_select
CFLAGS = -Wall -Werror -Wextra
LIBFT = libft/
CC = clang
SRC_NAME = choice.c\
		   draw.c\
		   list_cir.c\
		   main.c\
		   signal.c\
		   singleton.c\
		   term.c
SRC_PATH = srcs
SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(SRC:.c=.o)
RM = rm -f

.PHONY: all, clean, fclean, re

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -I includes -I libft/includes -lft -L libft -ltermcap

%.o: %.c
	@$(CC) $(CFLAGS) -c $^ -o $@ -I includes -I libft/includes

clean:
	@make -C libft clean
	@$(RM) $(OBJ)

fclean: clean
	@make -C libft fclean
	@$(RM) $(NAME)

re: fclean all
