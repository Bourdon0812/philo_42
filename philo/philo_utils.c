/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:30:46 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/04/06 22:14:59 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/***************************** Eat task (part 3) ******************************/
static int	eat3(t_philo *philo, t_data *data, int left, int right)
{
	if (philo->nb_meals == data->lim && data->lim != -1)
	{
		pthread_mutex_unlock(&data->forks[left]);
		pthread_mutex_unlock(&data->forks[right]);
		return (0);
	}
	print_task(philo, data, "is eating");
	pthread_mutex_lock(&data->time);
	philo->last_meal = get_current_time();
	philo->nb_meals++;
	pthread_mutex_unlock(&data->time);
	ft_wait(data, data->eat_time);
	pthread_mutex_unlock(&data->forks[left]);
	pthread_mutex_unlock(&data->forks[right]);
	return (1);
}

static int	eat2(t_philo *philo, t_data *data, int left, int right)
{
	if (philo->id % 2 == 1)
		pthread_mutex_lock(&data->forks[right]);
	else
		pthread_mutex_lock(&data->forks[left]);
	print_task(philo, data, "has taken a fork");
	pthread_mutex_lock(&data->is_dead);
	if (data->philo_dead)
	{
		pthread_mutex_unlock(&data->forks[left]);
		pthread_mutex_unlock(&data->forks[right]);
		pthread_mutex_unlock(&data->is_dead);
		return (0);
	}
	pthread_mutex_unlock(&data->is_dead);
	if (!eat3(philo, data, left, right))
		return (0);
	return (1);
}

int	eat(t_philo *philo, t_data *data)
{
	int		left;
	int		right;

	left = philo->id - 1;
	right = philo->id;
	if (philo->id == data->nb_philo)
		right = 0;
	pthread_mutex_lock(&data->is_dead);
	if (data->philo_dead)
		return (pthread_mutex_unlock(&data->is_dead), 0);
	pthread_mutex_unlock(&data->is_dead);
	if (philo->id % 2 == 1)
		pthread_mutex_lock(&data->forks[left]);
	else
		pthread_mutex_lock(&data->forks[right]);
	print_task(philo, data, "has taken a fork");
	if (!eat2(philo, data, left, right))
		return (0);
	return (1);
}

void	ft_sleep(t_philo *philo, t_data *data)
{
	print_task(philo, data, "is sleeping");
	ft_wait(data, data->sleep_time);
}

void	print_task(t_philo *philo, t_data *data, char *msg)
{
	long long	time;

	pthread_mutex_lock(&data->log);
	time = get_time(data->start_time);
	pthread_mutex_lock(&data->is_dead);
	if (data->philo_dead)
	{
		pthread_mutex_unlock(&data->log);
		pthread_mutex_unlock(&data->is_dead);
		return ;
	}
	pthread_mutex_unlock(&data->is_dead);
	printf("%lld\t", time);
	printf("%d %s", philo->id, msg);
	printf("\033[0;37m\n\033[0m");
	pthread_mutex_unlock(&data->log);
}
