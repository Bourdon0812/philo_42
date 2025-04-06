/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:07:58 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/04/05 00:18:20 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	destroy_forks(t_data *data, int count)
{
	while (--count >= 0)
		pthread_mutex_destroy(&data->forks[count]);
}

static void	cleanup_mutexes(t_data *data, int fork_count, int level)
{
	if (data->forks)
	{
		destroy_forks(data, fork_count);
		free(data->forks);
	}
	if (level >= 1)
		pthread_mutex_destroy(&data->log);
	if (level >= 2)
		pthread_mutex_destroy(&data->meal_check);
	if (level >= 3)
		pthread_mutex_destroy(&data->time);
	if (level >= 4)
		pthread_mutex_destroy(&data->is_dead);
}

static int	init_additional_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->time, NULL))
	{
		cleanup_mutexes(data, data->nb_philo, 2);
		return (0);
	}
	if (pthread_mutex_init(&data->is_dead, NULL))
	{
		cleanup_mutexes(data, data->nb_philo, 3);
		return (0);
	}
	return (1);
}

static int	init_shared_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->log, NULL))
	{
		cleanup_mutexes(data, data->nb_philo, 0);
		return (0);
	}
	if (pthread_mutex_init(&data->meal_check, NULL))
	{
		cleanup_mutexes(data, data->nb_philo, 1);
		return (0);
	}
	return (init_additional_mutexes(data));
}

int	init_mutex(t_data *data)
{
	int	i;

	data->forks = malloc(data->nb_philo * sizeof(pthread_mutex_t));
	if (data->forks == NULL)
		return (0);
	i = -1;
	while (++i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			destroy_forks(data, i);
			free(data->forks);
			return (0);
		}
	}
	return (init_shared_mutexes(data));
}
