/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:33:24 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/09 21:04:55 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_putstr_fd("Error: gettimeofday\n", 2);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	init_philos(t_philo *philos, t_sim *simulation, 
						pthread_mutex_t *forks, char **argv)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philos[i].id = i + 1;
		philos[i].eating = 0;
		philos[i].eat_count = 0;
		philos[i].start_time = get_time();
		philos[i].last_meal = get_time();
		philos[i].dead = &simulation->dead_flag;
		philos[i].right_fork = &forks[i];
		if (i == 0)
			philos[i].left_fork = &forks[philos[i].num_of_philos - 1];
		else
			philos[i].left_fork = &forks[i - 1];
		philos[i].print_lock = &simulation->print_lock;
		philos[i].dead_lock = &simulation->dead_lock;
		philos[i].meal_lock = &simulation->meal_lock;
		i++;
	}
}

void	init_forks(pthread_mutex_t *forks, int num_of_philos)
{
	int	i;

	i = 0;
	while (i < num_of_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	init_simulation(t_sim *simulation, t_philo *philos)
{
	simulation->dead_flag = 0;
	simulation->philos = philos;
	pthread_mutex_init(&simulation->dead_lock, NULL);
	pthread_mutex_init(&simulation->meal_lock, NULL);
	pthread_mutex_init(&simulation->print_lock, NULL);
}

void	init_input(t_philo *philo, char **argv)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philo[i].num_of_philos = ft_atoi(argv[1]);
		philo[i].time_to_die = ft_atoi(argv[2]);
		philo[i].time_to_eat = ft_atoi(argv[3]);
		philo[i].time_to_sleep = ft_atoi(argv[4]);
		philo[i].id = i + 1;
		if (argv[5])
			philo[i].times_must_eat = ft_atoi(argv[5]);
		else
			philo[i].times_must_eat = -1;
		i++;
	}
}
