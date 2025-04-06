/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:56:17 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/04/07 00:03:23 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "../libft/libft.h"
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <string.h>

typedef struct s_data
{
	int					nb_philo;
	int					death_time;
	int					eat_time;
	int					sleep_time;
	int					lim;
	int					philo_dead;
	long long			start_time;
	pthread_mutex_t		*forks;
	pthread_mutex_t		log;
	pthread_mutex_t		meal_check;
	pthread_mutex_t		time;
	pthread_mutex_t		is_dead;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				nb_meals;
	long long		last_meal;
	int				is_full;
	pthread_t		thread;
	t_data			*data;
}				t_philo;

/* time_utils.c */
long long	get_current_time(void);
long long	get_time(long long time);
void		ft_wait(t_data *data, long long time);

/* mutex.c */
int			init_mutex(t_data *data);

/* thread.h */
void		one_philo(t_data *data);
void		check_deaths(t_data *data, t_philo *philos);
int			init_threads(t_data *data, t_philo *philos);

/* philo_utils.c */
int			eat(t_philo *philo, t_data *data);
void		ft_sleep(t_philo *philo, t_data *data);
void		print_task(t_philo *philo, t_data *data, char *msg);

#endif
