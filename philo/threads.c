/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:11:23 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/29 21:28:57 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 *		- pthread_create: Creates a new thread.
 *      - pthread_detach: Detaches a thread.
 *      - pthread_join: Joins with a terminated thread.
 *      - pthread_mutex_init: Initializes a mutex.
 *      - pthread_mutex_destroy: Destroys a mutex.
 *      - pthread_mutex_lock: Locks a mutex.
 *      - pthread_mutex_unlock: Unlocks a mutex.
*/

void	destroy_mutexes(t_sim *simulation)
{
	int	i;

	pthread_mutex_destroy(&simulation->dead_lock);
	pthread_mutex_destroy(&simulation->meal_lock);
	pthread_mutex_destroy(&simulation->print_lock);
	i = 0;
	while (i < simulation->philo[0].philo_count)
	{
		pthread_mutex_destroy(simulation->philo[i].print_lock);
		i++;
	}
}

void	print_philo(t_philo *philo, char *str)
{
	pthread_mutex_lock(philo->print_lock);
	printf("%d Philosopher %d %s\n", get_time(), philo->id, str);
	pthread_mutex_unlock(philo->print_lock);
}

void	*monitor(void *philo_ptr)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)philo_ptr;
	while (1)
	{
		i = 0;
		while (i < philo[0].philo_count)
		{
			if (get_time() - philo[i].last_meal > philo[i].time_to_die)
			{
				pthread_mutex_lock(philo[i].print_lock);
				printf("%d Philosopher %d died\n", get_time(), philo[i].id);
				pthread_mutex_unlock(philo[i].print_lock);
				*philo[i].dead = 1;
				return (NULL);
			}
			i++;
		}
	}
	return (philo_ptr);
}

void	*philo_routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	while (1)
	{
		if (*philo->dead)
			return (NULL);
		pthread_mutex_lock(philo->left_fork);
		print_philo(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_philo(philo, "has taken a fork");
		philo->last_meal = get_time();
		print_philo(philo, "is eating");
		philo->eat_count++;
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		print_philo(philo, "is sleeping");
		usleep(philo->time_to_sleep * 1000);
		print_philo(philo, "is thinking");
	}
	return (philo_ptr);
}

//pthread_mutex_t *forks
int	create_threads(t_sim *simulation) 
{
	pthread_t	observer;
	int			i;

	pthread_create(&observer, NULL, &monitor, simulation->philo);
	i = 0;
	while (i < simulation->philo[0].philo_count)
	{
		pthread_create(&simulation->philo[i].thread, NULL, &philo_routine,
				&simulation->philo[i]);
		i++;
	}
	pthread_join(observer, NULL);
	i = 0;
	while (i < simulation->philo[0].philo_count)
	{
		pthread_join(simulation->philo[i].thread, NULL);
		i++;
	}
	return (0);
}