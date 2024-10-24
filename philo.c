/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: starscourge <starscourge@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:19:11 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/24 13:05:05 by starscourge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	now;
	size_t			time;

	gettimeofday(&now, NULL);
	time = now.tv_sec * 1000;
	time += now.tv_usec / 1000;
	return (time);
}

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

void	print_routine(t_philo *philo, char	*status)
{
	t_data	*data;

	data = philo->data;
	if (data->eat_count != -1 && philo->eat_count >= data->eat_count)
	{
		data->full = 1;
		return ;
	}
	if (data->dead == 1)
		return ;
	printf("%zu %d %s\n", get_time() - data->start, philo->id, status);
}

void	one_philo(t_data	*data)
{
	printf("%zu %d has taken a fork\n", get_time() - data->start, 1);
	while (1)
	{
		if (get_time() - data->start > data->time_to_die)
		{
			printf("%zu %d died\n", get_time() - data->start, 1);
			break ;
		}
	}
}

int	parse_args(t_data *data, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (1);
	if (check_args(ac, av))
		return (1);
	data->start = get_time();
	data->philo_count = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		data->eat_count = atoi(av[5]);
	else
		data->eat_count = -1;
	if (data->time_to_die < 60
		|| data->time_to_eat < 60 || data->time_to_sleep < 60
		|| (ac == 6 && data->eat_count < 1))
		return (1);
	if (data->philo_count < 2)
	{
		one_philo(data);
		return (2);
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
	data->full = 0;
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2)
		usleep(42);
	while (1)
	{
		pthread_mutex_lock(&data->print);
		if (data->dead == 1 || (data->eat_count != -1
				&& philo->eat_count >= data->eat_count))
		{
			pthread_mutex_unlock(&data->print);
			break ;
		}
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->print);
		print_routine(philo, "is thinking");
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		pthread_mutex_lock(&data->print);
		print_routine(philo, "has taken a fork");
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		pthread_mutex_lock(&data->print);
		print_routine(philo, "has taken a fork");
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->print);
		print_routine(philo, "is eating");
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->print);
		philo->last_meal = get_time() - data->start;
		pthread_mutex_unlock(&data->print);
		usleep(data->time_to_eat * 1000);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
		pthread_mutex_lock(&data->print);
		philo->eat_count++;
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->print);
		print_routine(philo, "is sleeping");
		pthread_mutex_unlock(&data->print);
		usleep(data->time_to_sleep * 1000);
	}
	return (NULL);
}

void	jalalat_badr(t_philo *philo)
{
	t_data	*data;
	int		i;
	int		stop_simulation;

	i = 0;
	stop_simulation = 0;
	while (1)
	{
		i = 0;
		while (i < philo->data->philo_count)
		{
			data = philo[i].data;
			pthread_mutex_lock(&data->print);
			if (data->eat_count != -1 && philo[i].eat_count >= data->eat_count)
			{
				stop_simulation = -1;
				pthread_mutex_unlock(&data->print);
				break ;
			}
			if (philo->eat_count == 0)
			{
				if (get_time() - data->start > data->time_to_die)
				{
					philo[i].data->dead = 1;
					printf("%zu %d died\n", get_time() - data->start, philo[i].id);
					stop_simulation = -1;
					pthread_mutex_unlock(&data->print);
					break ;
				}
			}
			else
			{
				if (get_time() - data->start - philo[i].last_meal > data->time_to_die)
				{
					philo[i].data->dead = 1;
					printf("%zu %d died\n", get_time() - data->start, philo[i].id);
					stop_simulation = -1;
					pthread_mutex_unlock(&data->print);
					break ;
				}
			}
			pthread_mutex_unlock(&data->print);
			i++;
		}
		if (stop_simulation == -1)
			break ;
	}
}

int	init_philo(t_philo *philo, t_data *data)
{
	int			i;
	pthread_t	monitor_routine;

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

int	main(int ac, char **av)
{
	t_data		data;
	t_philo		*philo;
	int			flag;


	flag = parse_args(&data, ac, av);
	if (flag)
	{
		if (flag == 1)
			return (printf("Error: wrong arguments\n"));
		else
			return (0);
	}
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
