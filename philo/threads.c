/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:12:58 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/05/11 23:42:31 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	full(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_check);
	philo->is_full = 1;
	pthread_mutex_unlock(&philo->data->meal_check);
}

static void	*philo_thread(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	if (philo->id % 2 == 0)
		usleep(25000);
	while (philo->nb_meals != philo->data->lim)
	{
		pthread_mutex_lock(&philo->data->is_dead);
		if (philo->data->philo_dead)
			return (pthread_mutex_unlock(&philo->data->is_dead), NULL);
		pthread_mutex_unlock(&philo->data->is_dead);
		if (!eat(philo, philo->data))
			break ;
		if (philo->data->lim != philo->nb_meals)
		{
			pthread_mutex_lock(&philo->data->is_dead);
			if (philo->data->philo_dead)
				return (pthread_mutex_unlock(&philo->data->is_dead), NULL);
			pthread_mutex_unlock(&philo->data->is_dead);
			ft_sleep(philo, philo->data);
			print_task(philo, philo->data, "is thinking");
		}
	}
	full(philo);
	return (NULL);
}

static int	check_philo_death(t_data *data, t_philo *philo)
{
	int	time_since_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	time_since_meal = get_time(philo->last_meal);
	if (data->lim != -1 && philo->nb_meals >= data->lim)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
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
	int	i = 0;
	int	full_count = 0;

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
	int	i = -1;

	while (++i < data->nb_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, &philo_thread, &philos[i]))
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
