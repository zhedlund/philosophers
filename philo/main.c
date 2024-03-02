/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:52:31 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/02 20:49:02 by zhedlund         ###   ########.fr       */
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

int	ft_usleep(unsigned int sleep_time_ms)
{
	unsigned int	start;

	start = get_time();
	while ((get_time() - start) < sleep_time_ms)
		usleep(500);
	return (0);
}

void	print_output(t_philo *philo, int id, char *str)
{
	size_t	time;

	time = get_time() - philo->start_time;
	pthread_mutex_lock(philo->print_lock);
	printf("%zu %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->print_lock);
}


void	philo_thinking(t_philo *philo)
{
	print_output(philo, philo->id, "is thinking");
}

// Dream routine funtion

void	philo_sleeping(t_philo *philo)
{
	print_output(philo, philo->id, "is sleeping");
	ft_usleep(philo->time_to_sleep);
}

// Eat routine funtion

void	philo_eating(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_output(philo, philo->id, "has taken a fork");
	if (philo->philo_count == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	pthread_mutex_lock(philo->left_fork);
	print_output(philo, philo->id, "has taken a fork");
	philo->eating = 1;
	print_output(philo, philo->id, "is eating");
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(philo->meal_lock);
	ft_usleep(philo->time_to_eat);
	philo->eating = 0;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void *philo_routine(void *pointer)
{
	t_philo *philo;
	int		i;

	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	i = 0;
	while (i < philo->philo_count) // change to death flag
	{
		philo_thinking(philo);
		philo_sleeping(philo);
		philo_eating(philo);
		i++;
	}
	return NULL;
}

void *monitor(void *ptr)
{
    t_philo *philo;
    int i = 0;

    philo = (t_philo *)ptr;
    while (i < philo->philo_count)
	{
        printf("Monitoring philosopher %d\n", philo->id);
		//print_output(philo, philo->id, "is being monitored");
        //ft_usleep(1);
        i++;
    }
    return NULL;
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
	while (i < simulation->philos[0].philo_count)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void create_threads(t_sim *simulation, pthread_mutex_t *forks)
{
    pthread_t	monitor_thread;
    pthread_t	philo_thread[MAX_PHILOS];
    int			i;

    if (pthread_create(&monitor_thread, NULL, monitor, simulation) != 0)
        destroy_mutexes(simulation, forks, "Failed to create monitor thread");
	i = 0;
	while (i < simulation->philos[0].philo_count)
	{
        if (pthread_create(&philo_thread[i], NULL, philo_routine, &simulation->philos[i]) != 0)
            destroy_mutexes(simulation, forks, "Failed to create philosopher thread");
        i++;
    }
    if (pthread_join(monitor_thread, NULL) != 0) {
        destroy_mutexes(simulation, forks, "Failed to join monitor thread");
    }
	printf("Joined monitor thread\n");
    while (i < simulation->philos[0].philo_count)
	{
        if (pthread_join(philo_thread[i], NULL) != 0)
            destroy_mutexes(simulation, forks, "Failed to join philosopher thread");
		i++;
    }
	printf("Joined philosopher threads\n");
}


void	init_philos(t_philo *philos, t_sim *simulation, pthread_mutex_t *forks, char **argv)
{
	int i;
	
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philos[i].id = i + 1;
		philos[i].philo_count = ft_atoi(argv[1]);
		philos[i].time_to_die = ft_atoi(argv[2]);
		philos[i].time_to_eat = ft_atoi(argv[3]);
		philos[i].time_to_sleep = ft_atoi(argv[4]);
		philos[i].start_time = get_time();
		philos[i].last_meal = get_time();
		if (argv[5])
			philos[i].num_times_to_eat = ft_atoi(argv[5]);
		else
			philos[i].num_times_to_eat = -1;
		philos[i].eating = 0;
		philos[i].eat_count = 0;
		philos[i].philo_dead = &simulation->dead_flag;
		philos[i].right_fork = &forks[i];
		if (i == 0)
			philos[i].left_fork = &forks[philos[i].philo_count - 1];
		else
			philos[i].left_fork = &forks[i - 1];
		philos[i].print_lock = &simulation->print_lock;
		philos[i].dead_lock = &simulation->dead_lock;
		philos[i].meal_lock = &simulation->meal_lock;
		i++;
	}
}

void	init_forks(pthread_mutex_t *forks, int philo_count)
{
	int	i;

	i = 0;
	while (i < philo_count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	init_simulation(t_sim *simulation, t_philo *philos)
{
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
	//init_simulation(philo, &simulation, forks, argv); // init structs
	init_simulation(&simulation, philos);
	init_forks(forks, ft_atoi(argv[1])); // init forks
	init_philos(philos, &simulation, forks, argv); // init philo structs
	printf("Philos in sim struct: %d\n", simulation.philos[0].philo_count);
	create_threads(&simulation, forks); // create threads, run loop
	destroy_mutexes(&simulation, forks, NULL); // destroy mutexes
	return (0);
}