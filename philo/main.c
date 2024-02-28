/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:52:31 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/28 22:42:14 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


/*
 * INPUT
 *
 * ./philo 5 800 200 200 [7]
*/

int	main(int argc, char **argv)
{
	t_sim			*simulation;
	t_philo			philo[MAX_PHILOS];
	pthread_mutex_t	forks[MAX_PHILOS];

	if (!is_valid_input(argc, argv))
		return (1);
	init_simulation(philo, &simulation, forks, argv); // init structs
	start_simulation(&simulation, forks); // create threads, run loop
	end_simulation() //destroy_mutexes
	return (0);
}