/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: starscourge <starscourge@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:13:06 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/25 13:34:30 by starscourge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	*philo_routine(void *arg)
// {
// 	t_philo	*philo;
// 	t_data	*data;

// 	philo = (t_philo *)arg;
// 	data = philo->data;
// 	if (philo->id % 2)
// 		usleep(42);
// 	while (1)
// 	{
// 		pthread_mutex_lock(&data->print);
// 		if (data->dead == 1 || (data->eat_count != (size_t)-1
// 				&& philo->eat_count >= data->eat_count))
// 		{
// 			pthread_mutex_unlock(&data->print);
// 			break ;
// 		}
// 		pthread_mutex_unlock(&data->print);
// 		pthread_mutex_lock(&data->print);
// 		print_routine(philo, "is thinking");
// 		pthread_mutex_unlock(&data->print);
// 		pthread_mutex_lock(&data->forks[philo->left_fork]);
// 		pthread_mutex_lock(&data->print);
// 		print_routine(philo, "has taken a fork");
// 		pthread_mutex_unlock(&data->print);
// 		pthread_mutex_lock(&data->forks[philo->right_fork]);
// 		pthread_mutex_lock(&data->print);
// 		print_routine(philo, "has taken a fork");
// 		pthread_mutex_unlock(&data->print);
// 		pthread_mutex_lock(&data->print);
// 		print_routine(philo, "is eating");
// 		pthread_mutex_unlock(&data->print);
// 		pthread_mutex_lock(&data->print);
// 		philo->last_meal = get_time() - data->start;
// 		pthread_mutex_unlock(&data->print);
// 		usleep(data->time_to_eat * 1000);
// 		pthread_mutex_unlock(&data->forks[philo->left_fork]);
// 		pthread_mutex_unlock(&data->forks[philo->right_fork]);
// 		pthread_mutex_lock(&data->print);
// 		philo->eat_count++;
// 		pthread_mutex_unlock(&data->print);
// 		pthread_mutex_lock(&data->print);
// 		print_routine(philo, "is sleeping");
// 		pthread_mutex_unlock(&data->print);
// 		usleep(data->time_to_sleep * 1000);
// 	}
// 	return (NULL);
// }

void	my_usleep(size_t time)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(1);
}

void	philo_think(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->print);
	print_routine(philo, "is thinking");
	pthread_mutex_unlock(&data->print);
}

void	philo_eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->forks[philo->left_fork]);
	pthread_mutex_lock(&data->print);
	print_routine(philo, "has taken a fork");
	pthread_mutex_unlock(&data->print);
	pthread_mutex_lock(&data->forks[philo->right_fork]);
	pthread_mutex_lock(&data->print);
	print_routine(philo, "has taken a fork");
	print_routine(philo, "is eating");
	philo->last_meal = get_time() - data->start;
	pthread_mutex_unlock(&data->print);
	my_usleep(data->time_to_eat);
	pthread_mutex_lock(&data->print);
	philo->eat_count++;
	pthread_mutex_unlock(&data->print);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

void	philo_sleep(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->print);
	print_routine(philo, "is sleeping");
	pthread_mutex_unlock(&data->print);
	my_usleep(data->time_to_sleep);
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
		if (data->dead == 1 || (data->eat_count != (size_t) -1
				&& philo->eat_count >= data->eat_count))
		{
			pthread_mutex_unlock(&data->print);
			break ;
		}
		pthread_mutex_unlock(&data->print);
		philo_think(philo);
		philo_eat(philo);
		philo_sleep(philo);
	}
	return (NULL);
}
