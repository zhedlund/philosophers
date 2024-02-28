/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:11:23 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/28 22:40:04 by zhedlund         ###   ########.fr       */
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


void	*philo_dinner(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (1)
	{
		if (philo->dead)
			return (NULL);
		pthread_mutex_lock(&philo->forks[philo->left_fork]);
		print_philo(philo, "has taken a fork");
		pthread_mutex_lock(&ph->forks[philo->right_fork]);
		print_philo(philo, "has taken a fork");
		philo->last_eat = get_time();
		print_philo(philo, "is eating");
		philo->eat_count++;
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		print_philo(philo, "is sleeping");
		usleep(philo->time_to_sleep * 1000);
		print_philo(philo, "is thinking");
	}
	return (NULL);
}

void	create_threads(t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < philo->philo_count)
	{
		pthread_create(&philo[i].thread, NULL, philo_dinner, &philo[i]);
		pthread_detach(philo[i].thread);
		i++;
	}
}
