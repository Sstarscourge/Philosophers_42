# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: starscourge <starscourge@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 12:19:09 by starscourge       #+#    #+#              #
#    Updated: 2024/10/25 13:49:44 by starscourge      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

NAME = philosophers
SRCS = philo.c parse_and_initialize.c routine.c utils.c
		
OBJS = ${SRCS:.c=.o}

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:  %.c philo.h
	$(CC) $(CFLAGS) -c  $< -o $@  
clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
