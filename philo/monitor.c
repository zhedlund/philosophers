/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:11:33 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/28 22:40:53 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
*/
void	print_philo(t_philo *philo, char *str)
{
	pthread_mutex_lock(philo->print);
	printf("%ld Philosopher %d %s\n", get_time(), philo->id, str);
	pthread_mutex_unlock(philo->print);
}

void	*death_monitor(t_philo *philo)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < philo->philo_count)
		{
			if (get_time() - philo[i].last_meal > philo->time_to_die)
			{
				print_philo(&philo[i], "died");
				philo[i].dead = 1;
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}
