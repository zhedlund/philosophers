/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:52:31 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/09 21:13:38 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_sim			simulation;
	t_philo			philos[MAX_PHILOS];
	pthread_mutex_t	forks[MAX_PHILOS];

	if (!is_valid_input(argc, argv))
		return (1);
	init_input(philos, argv);
	init_simulation(&simulation, philos);
	init_forks(forks, ft_atoi(argv[1]));
	init_philos(philos, &simulation, forks, argv);
	create_threads(&simulation);
	destroy_mutexes(&simulation, forks);
	return (0);
}
