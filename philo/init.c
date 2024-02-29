/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:33:24 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/29 21:22:11 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	init_input(t_philo *philo, char **argv)
{
	philo->philo_count = ft_atoi(argv[1]);
	philo->time_to_die = ft_atoi(argv[2]);
	philo->time_to_eat = ft_atoi(argv[3]);
	philo->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		philo->times_to_eat = ft_atoi(argv[5]);
	else
		philo->times_to_eat = -1;
}

void	init_philo(t_philo *philo, t_sim **simulation, pthread_mutex_t *forks,
					char **argv)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		init_input(&philo[i], argv);
		philo[i].id = i + 1;
		philo[i].eating = 0;
		philo[i].eat_count = 0;
		philo[i].start_time = get_time();
		philo[i].last_meal = get_time();
		philo[i].dead_lock = &(*simulation)->dead_lock;
		philo[i].meal_lock = &(*simulation)->meal_lock;
		philo[i].print_lock = &(*simulation)->print_lock;
		philo[i].dead = &(*simulation)->dead_flag;
		philo[i].left_fork = &forks[i];
		if (i == 0)
			philo[i].right_fork = &forks[philo[i].philo_count - 1];
		else
			philo[i].right_fork = &forks[i - 1];
		i++;
	}
}

void	init_forks(pthread_mutex_t *forks, int philo_count)
{
	int	i;

	i = 0;
	while (i < philo_count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	init_simulation(t_philo *philo, t_sim **simulation,
							pthread_mutex_t *forks, char **argv)
{	
	(*simulation)->dead_flag = 0;
	(*simulation)->philo = philo;
	pthread_mutex_init(&(*simulation)->dead_lock, NULL);
	pthread_mutex_init(&(*simulation)->meal_lock, NULL);
	pthread_mutex_init(&(*simulation)->print_lock, NULL);
	init_forks(forks, ft_atoi(argv[1]));
	init_philo(philo, simulation, forks, argv);
}

