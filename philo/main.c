/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:52:31 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/03 20:54:36 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_putstr_fd("Error: gettimeofday\n", 2);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_usleep(unsigned int time_ms)
{
	unsigned int	start;

	start = get_time();
	while ((get_time() - start) < time_ms)
		usleep(500);
	return (0);
}

void	print_output(t_philo *philo, char *str)
{
	size_t	time;

	pthread_mutex_lock(philo->print_lock);
	time = get_time() - philo->start_time;
	printf("%zu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(philo->print_lock);
}


void *philo_routine(void *ptr)
{
	t_philo *philo;
	int		i;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	i = 0;
	while (i < 5) // change to death flag
	{
		printf("%d is alive\n", philo->id);
		//ft_usleep(1);
		i++;
	}
	return (ptr);
}

void *monitor(void *ptr)
{
    t_philo *philo;
    int i = 0;

    philo = (t_philo *)ptr;
    while (i < 5)
	{
		printf("%d is being monitored\n", philo->id);
        i++;
    }
	return (ptr);
}


void	destroy_mutexes(t_sim *simulation, pthread_mutex_t *forks, char *str)
{
	int	i;

	i = 0;
	if (str)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd("\n", 2);
	}
	pthread_mutex_destroy(&simulation->dead_lock);
	pthread_mutex_destroy(&simulation->meal_lock);
	pthread_mutex_destroy(&simulation->print_lock);
	while (i < simulation->num_of_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void create_threads(t_sim *simulation, pthread_mutex_t *forks)
{
    int			i;

    if (pthread_create(&simulation->monitor, NULL, monitor, simulation) != 0)
        destroy_mutexes(simulation, forks, "Failed to create monitor thread");
	i = 0;
	while (i < simulation->num_of_philos)
	{
        if (pthread_create(&simulation->philos[i].thread, NULL, philo_routine, &simulation->philos[i]) != 0)
            destroy_mutexes(simulation, forks, "Failed to create philosopher thread");
        i++;
    }
    if (pthread_join(simulation->monitor, NULL) != 0) {
        destroy_mutexes(simulation, forks, "Failed to join monitor thread");
    }
	printf("Joined monitor thread\n");
    while (i < simulation->num_of_philos)
	{
        if (pthread_join(simulation->philos[i].thread, NULL) != 0)
            destroy_mutexes(simulation, forks, "Failed to join philosopher thread");
		i++;
    }
	printf("Joined philosopher threads\n");
}


void	init_philos(t_philo *philos, t_sim *simulation, pthread_mutex_t *forks)
{
	int i;
	
	i = 0;
	while (i < simulation->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].start_time = get_time();
		philos[i].last_meal = get_time();
		philos[i].eating = 0;
		philos[i].eat_count = 0;
		philos[i].philo_dead = &simulation->dead_flag;
		philos[i].right_fork = &forks[i];
		if (i == 0)
			philos[i].left_fork = &forks[simulation->num_of_philos - 1];
		else
			philos[i].left_fork = &forks[i - 1];
		philos[i].print_lock = &simulation->print_lock;
		philos[i].dead_lock = &simulation->dead_lock;
		philos[i].meal_lock = &simulation->meal_lock;
		i++;
	}
}

void	init_forks(pthread_mutex_t *forks, int num_of_philos)
{
	int	i;

	i = 0;
	while (i < num_of_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	init_simulation(t_sim *simulation, t_philo *philos, char **argv)
{
	simulation->num_of_philos = ft_atoi(argv[1]);
	simulation->time_to_die = ft_atoi(argv[2]);
	simulation->time_to_eat = ft_atoi(argv[3]);
	simulation->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		simulation->times_must_eat = ft_atoi(argv[5]);
	else
		simulation->times_must_eat = -1;
	simulation->dead_flag = 0;
	simulation->philos = philos;
	pthread_mutex_init(&simulation->dead_lock, NULL);
	pthread_mutex_init(&simulation->meal_lock, NULL);
	pthread_mutex_init(&simulation->print_lock, NULL);
}

/*
 * INPUT
 *
 * ./philo 5 800 200 200 [7]
*/

int	main(int argc, char **argv)
{
	t_sim			simulation;
	t_philo			philos[MAX_PHILOS]; // philo struct array
	pthread_mutex_t	forks[MAX_PHILOS]; //mutex fork array

	if (!is_valid_input(argc, argv))
		return (1);
	init_simulation(&simulation, philos, argv);
	init_forks(forks, ft_atoi(argv[1])); // init forks
	init_philos(philos, &simulation, forks); // init philo structs
	printf("Philos in sim struct: %d\n", simulation.num_of_philos);
	create_threads(&simulation, forks); // create threads, run loop
	write(1, "Created threads\n", 17);
	destroy_mutexes(&simulation, forks, NULL); // destroy mutexes
	write(1, "Destroyed mutexes\n", 19);
	return (0);
}