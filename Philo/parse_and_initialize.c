/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_initialize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fidriss <fidriss@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:11:07 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/31 12:34:31 by fidriss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_data *data, int ac, char **av)
{
	pthread_t	thread;

	if (ac < 5 || ac > 6)
		return (1);
	if (check_args(ac, av))
		return (1);
	data->start = get_time();
	data->philo_count = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->eat_count = ft_atoi(av[5]);
	else
		data->eat_count = -1;
	if (data->philo_count < 1 || (ac == 6 && data->eat_count < 1))
		return (1);
	if (data->philo_count == 1)
	{
		pthread_create(&thread, NULL, (void *)one_philo, data);
		pthread_join(thread, NULL);
		return (1);
	}
	return (0);
}

int	init_data(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->print, NULL))
		return (1);
	data->dead = 0;
	return (0);
}

int	init_philo(t_philo *philo, t_data *data)
{
	int			i;

	i = 0;
	while (i < data->philo_count)
	{
		philo[i].id = i + 1;
		philo[i].left_fork = i;
		philo[i].right_fork = (i + 1) % data->philo_count;
		philo[i].eat_count = 0;
		philo[i].data = data;
		philo[i].last_meal = 0;
		if (pthread_create(&philo[i].thread, NULL, &philo_routine, &philo[i]))
			return (1);
		i++;
	}
	jalalat_badr(philo);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_join(philo[i].thread, NULL))
			return (1);
		i++;
	}
	return (0);
}

void	one_philo(t_data	*data)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * 1);
	if (!data->forks)
		return ;
	pthread_mutex_init(&data->forks[0], NULL);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_lock(&data->forks[0]);
	pthread_mutex_lock(&data->print);
	printf("%zu %d has taken a fork\n", get_time() - data->start, 1);
	while (1)
	{
		if (get_time() - data->start > data->time_to_die)
		{
			printf("%zu %d died\n", get_time() - data->start, 1);
			pthread_mutex_unlock(&data->forks[0]);
			pthread_mutex_unlock(&data->print);
			pthread_mutex_destroy(&data->forks[0]);
			pthread_mutex_destroy(&data->print);
			break ;
		}
	}
}
