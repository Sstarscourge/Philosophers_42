# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fidriss <fidriss@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 12:19:09 by starscourge       #+#    #+#              #
#    Updated: 2024/10/31 12:37:01 by fidriss          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = philo
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
