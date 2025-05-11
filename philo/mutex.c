/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:07:58 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/11 21:36:26 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	init_mutex3(t_data *data)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&data->time, NULL))
	{
		free(data->forks);
		while (++i < data->nb_philo)
			pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->log);
		pthread_mutex_destroy(&data->meal_check);
		return (0);
	}
	if (pthread_mutex_init(&data->is_dead, NULL))
	{
		free(data->forks);
		while (++i < data->nb_philo)
			pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->log);
		pthread_mutex_destroy(&data->meal_check);
		pthread_mutex_destroy(&data->time);
		return (0);
	}
	return (1);
}

static int	init_mutex2(t_data *data)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&data->log, NULL))
	{
		free(data->forks);
		while (++i < data->nb_philo)
			pthread_mutex_destroy(&data->forks[i]);
		return (0);
	}
	if (pthread_mutex_init(&data->meal_check, NULL))
	{
		free(data->forks);
		while (++i < data->nb_philo)
			pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->log);
		return (0);
	}
	if (!init_mutex3(data))
		return (0);
	return (1);
}

int	init_mutex(t_data *data)
{
	int	i;

	data->forks = malloc(data->nb_philo * sizeof(pthread_mutex_t));
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			free(data->forks);
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			return (0);
		}
		i++;
	}
	if (!init_mutex2(data))
		return (0);
	return (1);
}
