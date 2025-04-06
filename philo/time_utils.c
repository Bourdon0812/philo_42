/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:45:43 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/04/06 22:02:26 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long long	get_time(long long time)
{
	return (get_current_time() - time);
}

void	ft_wait(t_data *data, long long time)
{
	long long	start_time;
	int			is_dead;

	start_time = get_current_time();
	pthread_mutex_lock(&data->is_dead);
	is_dead = data->philo_dead;
	pthread_mutex_unlock(&data->is_dead);
	while (!is_dead && get_current_time() - start_time < time)
	{
		pthread_mutex_lock(&data->is_dead);
		is_dead = data->philo_dead;
		pthread_mutex_unlock(&data->is_dead);
		usleep(50);
	}
}
