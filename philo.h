/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: starscourge <starscourge@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:19:15 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/19 12:39:32 by starscourge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count;
	int				dead;
	int				full;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	struct timeval	start;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				eat_count;
	int				last_meal;
	pthread_t		thread;
	struct s_data	*data;
}					t_philo;

#endif