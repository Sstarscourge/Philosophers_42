/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: starscourge <starscourge@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:19:11 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/25 00:42:44 by starscourge      ###   ########.fr       */
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
			if (data->eat_count != (size_t)-1 && philo[i].eat_count >= data->eat_count)
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
	destroy_all(&data);
	free(data.forks);
	free(philo);
	return (0);
}
