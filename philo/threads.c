/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:11:23 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/09 21:13:49 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(t_sim *simulation, pthread_mutex_t *forks)
{
	int	i;

	pthread_mutex_destroy(&simulation->dead_lock);
	pthread_mutex_destroy(&simulation->meal_lock);
	pthread_mutex_destroy(&simulation->print_lock);
	i = 0;
	while (i < simulation->philos[0].num_of_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	*monitor_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (1)
	{
		if (check_if_dead(philo) == 1 || check_if_all_ate(philo) == 1)
			break ;
	}
	return (ptr);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!dead_flag(philo))
	{
		philo_thinking(philo);
		philo_eating(philo);
		philo_sleeping(philo);
	}
	return (ptr);
}

void	join_threads(t_sim *simulation, pthread_t monitor_thread)
{
	int	i;

	if (pthread_join(monitor_thread, NULL) != 0)
	{
		ft_putstr_fd("Error joining monitor thread\n", 2);
		return ;
	}
	i = 0;
	while (i < simulation->philos[0].num_of_philos)
	{
		if (pthread_join(simulation->philos[i].thread, NULL) != 0)
		{
			ft_putstr_fd("Error joining philo thread\n", 2);
			return ;
		}
		i++;
	}
}

void	create_threads(t_sim *simulation)
{
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	if (pthread_create(&monitor_thread, NULL, monitor_routine,
			simulation->philos) != 0)
	{
		ft_putstr_fd("Error creating monitor thread\n", 2);
		return ;
	}
	while (i < simulation->philos[0].num_of_philos)
	{
		if (pthread_create(&simulation->philos[i].thread, NULL, philo_routine,
				&simulation->philos[i]) != 0)
		{
			ft_putstr_fd("Error creating philo thread\n", 2);
			return ;
		}
		i++;
	}
	join_threads(simulation, monitor_thread);
}
