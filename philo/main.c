/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:53:18 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/04/07 00:10:00 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_errors(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
		return (0);
	i = 0;
	while (argv[++i] != NULL)
	{
		j = -1;
		while (argv[i][++j] != '\0')
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (0);
	}
	return (1);
}

void	init_data(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->death_time = ft_atoi(argv[2]);
	data->eat_time = ft_atoi(argv[3]);
	data->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		data->lim = ft_atoi(argv[5]);
	else
		data->lim = -1;
	data->philo_dead = 0;
	data->start_time = get_current_time();
}

void	init_philo(t_philo *philo, t_data *data)
{
	int	i;

	data->start_time = get_current_time();
	i = -1;
	while (++i < data->nb_philo)
	{
		memset(&philo[i], 0, sizeof(t_philo));
		philo[i].data = data;
		philo[i].id = i + 1;
		philo[i].nb_meals = 0;
		philo[i].last_meal = data->start_time;
	}
}

void	destroy_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->log);
	pthread_mutex_destroy(&data->meal_check);
	pthread_mutex_destroy(&data->time);
	pthread_mutex_destroy(&data->is_dead);
	free(data->forks);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philo;

	if (!check_errors(argc, argv))
		return (ft_putstr_fd("args error", 2), -1);
	init_data(&data, argc, argv);
	philo = malloc(data.nb_philo * sizeof(t_philo));
	if (philo == NULL)
		return (-1);
	init_philo(philo, &data);
	if (data.nb_philo == 1)
		return (one_philo(&data), free(philo), 0);
	if (!init_mutex(&data))
		return (free(philo), -1);
	if (!init_threads(&data, philo))
		return (destroy_mutex(&data), free(philo), -1);
	destroy_mutex(&data);
	free(philo);
	return (0);
}
