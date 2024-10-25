/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: starscourge <starscourge@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:10:08 by starscourge       #+#    #+#             */
/*   Updated: 2024/10/25 00:35:10 by starscourge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	checksign(const char *str, int i, int sign)
{
	if (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i] == '-')
			sign *= -1;
	}
	return (sign);
}

long long	ft_atoi(const char *str)
{
	int			i;
	int			sign;
	long long	result;
	long long	tmp;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	sign = checksign(str, i, sign);
	if (str[i] && (str[i] == '+' || str[i] == '-'))
		i++;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
		return (0);
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		tmp = result;
		result = result * 10 + (str[i++] - '0');
		if (result / 10 != tmp && sign == -1)
			return (0);
		if (result / 10 != tmp)
			return (-1);
	}
	return (result * sign);
}

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

void	destroy_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print);
}
