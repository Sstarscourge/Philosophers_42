/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: starscourge <starscourge@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:19:11 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/25 22:12:06 by starscourge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_routine(t_philo *philo, char	*status)
{
	t_data	*data;

	data = philo->data;
	if (data->eat_count != (size_t)-1 && philo->eat_count >= data->eat_count)
	{
		data->full = 1;
		return ;
	}
	if (data->dead == 1)
		return ;
	printf("%zu %d %s\n", get_time() - data->start, philo->id, status);
}

int	check_victim_or_full(t_data *data, t_philo	*philo)
{
	if (data->eat_count != (size_t)-1
		&& philo->eat_count >= data->eat_count)
		return (1);
	if (philo->eat_count == 0)
	{
		if (get_time() - data->start > data->time_to_die)
		{
			philo->data->dead = 1;
			printf("%zu %d died\n", get_time() - data->start, philo->id);
			return (1);
		}
	}
	else
	{
		if (get_time() - data->start
			- philo->last_meal > data->time_to_die)
		{
			philo->data->dead = 1;
			printf("%zu %d died\n", get_time() - data->start, philo->id);
			return (1);
		}
	}
	return (0);
}

void	jalalat_badr(t_philo *philo)
{
	t_data	*data;
	int		i;

	i = 0;
	while (1)
	{
		i = 0;
		while (i < philo->data->philo_count)
		{
			data = philo[i].data;
			pthread_mutex_lock(&data->print);
			if (check_victim_or_full(data, &philo[i]))
			{
				pthread_mutex_unlock(&data->print);
				return ;
			}
			pthread_mutex_unlock(&data->print);
			i++;
		}
	}
}

int	main(int ac, char **av)
{
	t_data		data;
	t_philo		*philo;

	if (parse_args(&data, ac, av))
		return (0);
	philo = malloc(sizeof(t_philo) * data.philo_count);
	if (!philo)
		return (printf("Error: malloc failed\n"));
	if (init_data(&data))
		return (printf("Error: mutex init failed\n"));
	if (init_philo(philo, &data))
		return (printf("Error: thread init failed\n"));
	destroy_all(&data);
	free(data.forks);
	free(philo);
	return (0);
}
