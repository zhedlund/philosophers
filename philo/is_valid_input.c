/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 18:25:36 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/28 21:29:56 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]
number_of_philosophers: The number of philosophers and also the number
of forks.
◦ time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
milliseconds since the beginning of their last meal or the beginning of the sim-
ulation, they die.
◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
◦ number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.

 ./philo 5 800 200 200 [7]*/

static int	check_args(int argc, char **argv)
{
	int i;
	int j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
			{
				printf("Error: arguments must be numbers\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	is_valid_input(int argc, char **argv)
{
	if (!check_args(argc, argv))
		return (0);
	if (argc < 5 || argc > 6)
	{
		printf("Error: number of arguments must be between 5 and 6\n");
		return (0);
	}
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[1]) > MAX_PHILOS)
	{
		printf("Error: number of philosophers must be between 2 and 200\n");
		return (0);
	}
	if (ft_atoi(argv[2]) < 1 || ft_atoi(argv[3]) < 1 || ft_atoi(argv[4]) < 1)
	{
		printf("Error: time_to_die, time_to_eat and time_to_sleep must be at least 1\n");
		return (0);
	}
	if (argc == 6 && ft_atoi(argv[5]) < 0)
	{
		printf("Error: number_of_times_each_philosopher_must_eat must be at least 1\n");
		return (0);
	}
	return (1);
}