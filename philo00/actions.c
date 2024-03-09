/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 20:26:40 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/09 20:50:02 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_output(t_philo *philo, char *str)
{
	int	time;

	pthread_mutex_lock(philo->print_lock);
	time = get_time() - philo->start_time;
	if (!dead_flag(philo))
		printf("%u %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(philo->print_lock);
}

void	philo_thinking(t_philo *philo)
{
	print_output(philo, "is thinking");
	ft_usleep(1);
}

void	philo_sleeping(t_philo *philo)
{
	print_output(philo, "is sleeping");
	ft_usleep(philo->time_to_sleep);
}

void	handle_solo_philo(t_philo *philo)
{
	ft_usleep(philo->time_to_die);
	if (philo->id % 2 == 0)
		pthread_mutex_unlock(philo->left_fork);
	else
		pthread_mutex_unlock(philo->right_fork);
}

void	philo_eating(t_philo *philo)
{
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->left_fork);
	else
		pthread_mutex_lock(philo->right_fork);
	print_output(philo, "has taken a fork");
	if (philo->num_of_philos == 1)
	{
		handle_solo_philo(philo);
		return ;
	}
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
	print_output(philo, "has taken a fork");
	philo->eating = 1;
	print_output(philo, "is eating");
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(philo->meal_lock);
	ft_usleep(philo->time_to_eat);
	philo->eating = 0;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}
