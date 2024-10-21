/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: starscourge <starscourge@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:19:11 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/20 17:36:54 by starscourge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_args(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	parse_args(t_data *data, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (1);
	if (check_args(ac, av))
		return (1);
	data->philo_count = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		data->eat_count = atoi(av[5]);
	else
		data->eat_count = -1;
	if (data->philo_count < 2 || data->time_to_die < 60
		|| data->time_to_eat < 60 || data->time_to_sleep < 60
		|| (ac == 6 && data->eat_count < 1))
		return (1);
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
	gettimeofday(&data->start, NULL);
	data->dead = 0;
	data->full = 0;
	return (0);
}

int	get_time(struct timeval start)
{
	struct timeval	now;
	int				time;

	gettimeofday(&now, NULL);
	time = (now.tv_sec - start.tv_sec) * 1000;
	time += (now.tv_usec - start.tv_usec) / 1000;
	return (time);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int		time;

	philo = (t_philo *)arg;
	data = philo->data;
	while (1)
	{
		pthread_mutex_lock(&data->print);
		if (data->dead)
		{
			pthread_mutex_unlock(&data->print);
			return (NULL);
		}
		pthread_mutex_unlock(&data->print);
		time = get_time(data->start);
		pthread_mutex_lock(&data->print);
		printf("%d %d is thinking\n", time, philo->id);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		time = get_time(data->start);
		pthread_mutex_lock(&data->print);
		printf("%d %d has taken a fork\n", time, philo->id);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		time = get_time(data->start);
		pthread_mutex_lock(&data->print);
		printf("%d %d has taken a fork\n", time, philo->id);
		pthread_mutex_unlock(&data->print);
		time = get_time(data->start);
		pthread_mutex_lock(&data->print);
		printf("%d %d is eating\n", time, philo->id);
		philo->last_meal = time;
		pthread_mutex_unlock(&data->print);
		usleep(data->time_to_eat * 1000);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
		pthread_mutex_lock(&data->print);
		philo->eat_count++;
		if (data->eat_count != -1 && philo->eat_count >= data->eat_count)
		{
			data->full = 1;
			pthread_mutex_unlock(&data->print);
			return (NULL);
		}
		pthread_mutex_unlock(&data->print);
		time = get_time(data->start);
		pthread_mutex_lock(&data->print);
		printf("%d %d is sleeping\n", time, philo->id);
		pthread_mutex_unlock(&data->print);
		usleep(data->time_to_sleep * 1000);
		pthread_mutex_lock(&data->print);
		if (get_time(data->start) - philo->last_meal > data->time_to_die)
		{
			printf("%d %d has died\n", get_time(data->start), philo->id);
			data->dead = 1;
			pthread_mutex_unlock(&data->print);
			return (NULL);
		}
		pthread_mutex_unlock(&data->print);
	}
	return (NULL);
}

int	init_philo(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		philo[i].id = i + 1;
		philo[i].left_fork = i;
		philo[i].right_fork = (i + 1) % data->philo_count;
		philo[i].eat_count = 0;
		philo[i].data = data;
		if (pthread_create(&philo[i].thread, NULL, &philo_routine, &philo[i]))
			return (1);
		i++;
	}
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_join(philo[i].thread, NULL))
			return (1);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data		data;
	t_philo		*philo;

	if (parse_args(&data, ac, av))
		return (printf("Error: wrong arguments\n"));
	philo = malloc(sizeof(t_philo) * data.philo_count);
	if (!philo)
		return (printf("Error: malloc failed\n"));
	if (init_data(&data))
		return (printf("Error: mutex init failed\n"));
	if (init_philo(philo, &data))
		return (printf("Error: thread init failed\n"));
	free(data.forks);
	free(philo);
	return (0);
}
