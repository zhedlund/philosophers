/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:52:31 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/29 21:27:34 by zhedlund         ###   ########.fr       */
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
	create_threads(simulation); // create threads, run loop
	destroy_mutexes(simulation); // destroy mutexes
	return (0);
}