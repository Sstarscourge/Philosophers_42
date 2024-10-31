/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fidriss <fidriss@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:19:15 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/25 22:57:29 by fidriss          ###   ########.fr       */
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
	size_t			start;
	int				philo_count;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			eat_count;
	int				dead;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	size_t			eat_count;
	size_t			last_meal;
	pthread_t		thread;
	struct s_data	*data;
}					t_philo;

long long	ft_atoi(const char *str);
size_t		get_time(void);
int			check_args(int ac, char **av);
void		destroy_all(t_data *data);
int			parse_args(t_data *data, int ac, char **av);
int			init_data(t_data *data);
int			init_philo(t_philo *philo, t_data *data);
void		*philo_routine(void *arg);
void		one_philo(t_data	*data);
void		jalalat_badr(t_philo *philo);
void		print_routine(t_philo *philo, char	*status);

#endif