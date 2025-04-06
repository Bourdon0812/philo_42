/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:12:58 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/04/06 22:33:32 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	one_philo(t_data *data)
{
	printf("coucouc\n");
	printf("%lld\t%d has taken a fork \n", get_time(data->start_time), 1);
	usleep(data->death_time * 1000);
	printf("%lld\t%d has died\n", get_time(data->start_time), 1);
}

static void	*philo(void *arg)
{
	t_thread_args	*a;

	a = (t_thread_args *)arg;
	if (a->philo->id % 2 == 0)
		usleep(25000);
	while (1)
	{
		if (!eat(a->philo, a->data))
			break ;
		if (a->philo->nb_meals == a->data->lim && a->data->lim != -1)
			break ;
		pthread_mutex_lock(&(a->data)->is_dead);
		if (a->data->philo_dead)
		{
			pthread_mutex_unlock(&(a->data)->is_dead);
			break ;
		}
		pthread_mutex_unlock(&(a->data)->is_dead);
		ft_sleep(a->philo, a->data);
		print_task(a->philo, a->data, "is thinking");
	}
	pthread_mutex_lock(&(a->data)->meal_check);
	a->philo->is_full = 1;
	pthread_mutex_unlock(&(a->data)->meal_check);
	return (free(a), NULL);
}

static int	check_philo_death(t_data *data, t_philo *philo)
{
	int	time_since_meal;

	pthread_mutex_lock(&data->time);
	time_since_meal = get_time(philo->last_meal);
	pthread_mutex_unlock(&data->time);
	if (data->lim != -1 && philo->nb_meals >= data->lim)
		return (0);
	if (time_since_meal > data->death_time)
	{
		print_task(philo, data, "has died");
		pthread_mutex_lock(&data->is_dead);
		data->philo_dead = 1;
		pthread_mutex_unlock(&data->is_dead);
		return (1);
	}
	return (0);
}

void	check_deaths(t_data *data, t_philo *philos)
{
	int	i;
	int	full_count;

	i = 0;
	full_count = 0;
	while (full_count < data->nb_philo)
	{
		usleep(50);
		if (i >= data->nb_philo)
			i = 0;
		if (check_philo_death(data, &philos[i]))
			break ;
		pthread_mutex_lock(&data->meal_check);
		if (philos[i].is_full)
		{
			full_count++;
			philos[i].is_full = 0;
		}
		pthread_mutex_unlock(&data->meal_check);
		i++;
	}
}

int	init_threads(t_data *data, t_philo *philos)
{
	int				i;
	t_thread_args	*args;

	i = -1;
	while (++i < data->nb_philo)
	{
		args = malloc(sizeof(t_thread_args));
		if (!args)
			return (0);
		args->philo = &philos[i];
		args->data = data;
		if (pthread_create(&philos[i].thread, NULL, philo, args))
			return (0);
		usleep(10);
	}
	check_deaths(data, philos);
	i = -1;
	while (++i < data->nb_philo)
	{
		if (pthread_join(philos[i].thread, NULL))
			return (0);
		usleep(10);
	}
	return (1);
}
