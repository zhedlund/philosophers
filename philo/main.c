/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:52:31 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/03 23:19:35 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_time(void)
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
	int	time;

	pthread_mutex_lock(philo->print_lock);
	time = get_time() - philo->start_time;
	printf("%d %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(philo->print_lock);
}

int deadlock(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->dead == 1)
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	pthread_mutex_unlock(philo->dead_lock);
	return (0);
}

int philo_died(t_philo *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	if (get_time() - philo->last_meal >= philo->time_to_die && philo->eating == 0)
	{
		pthread_mutex_unlock(philo->meal_lock);
		print_output(philo, "died");
		pthread_mutex_lock(philo->dead_lock);
		*philo->dead = 1;
		return(pthread_mutex_unlock(philo->dead_lock), 1);
	}
	pthread_mutex_unlock(philo->meal_lock);
	return (0);
}

int	philosopher_dead(t_philo *philo, int time_to_die)
{
	pthread_mutex_lock(philo->meal_lock);
	if (get_time() - philo->last_meal >= time_to_die
		&& philo->eating == 0)
		return (pthread_mutex_unlock(philo->meal_lock), 1);
	pthread_mutex_unlock(philo->meal_lock);
	return (0);
}

int	check_if_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].num_of_philos)
	{
		if (philosopher_dead(&philos[i], philos[i].time_to_die))
		{
			print_output(&philos[i], "died");
			pthread_mutex_lock(philos[0].dead_lock);
			*philos->dead = 1;
			pthread_mutex_unlock(philos[0].dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_if_all_ate(t_philo *philos)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].times_must_eat == -1)
		return (0);
	while (i < philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[i].meal_lock);
		if (philos[i].eat_count >= philos[i].times_must_eat)
			finished_eating++;
		pthread_mutex_unlock(philos[i].meal_lock);
		i++;
	}
	if (finished_eating == philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos->dead = 1;
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	return (0);
}




//int finished_eating()

void *philo_routine(void *ptr)
{
	t_philo *philo;
	int		i;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	i = 0;
	while (!deadlock(philo))
	{
		pthread_mutex_lock(philo->left_fork);
		print_output(philo, "has taken a fork");
		if (philo->num_of_philos == 1)
		{
			ft_usleep(philo->time_to_die);
			pthread_mutex_unlock(philo->left_fork);
			return (NULL);
		}
		pthread_mutex_lock(philo->right_fork);
		print_output(philo, "has taken a fork");
		philo->eating = 1;
		print_output(philo, "is eating");
		pthread_mutex_lock(philo->meal_lock);
		philo->last_meal = get_time();
		philo->eat_count++;
		pthread_mutex_unlock(philo->meal_lock);
		ft_usleep(philo->time_to_eat);
		philo->eating = 0;
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		print_output(philo, "is sleeping");
		ft_usleep(philo->time_to_sleep);
		print_output(philo, "is thinking");
		i++;
	}
	return (NULL);
}

void *monitor(void *ptr)
{
    t_philo *philo;
    int i = 0;

    philo = (t_philo *)ptr;
    while (1)
	{
		if (check_if_dead(philo) == 1 || check_if_all_ate(philo) == 1)
			break ;
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
	while (i < simulation->philos[0].num_of_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void create_threads(t_sim *simulation, pthread_mutex_t *forks)
{
    int			i;

	i = 0;
	while (i < simulation->philos[0].num_of_philos)
	{
        if (pthread_create(&simulation->philos[i].thread, NULL, philo_routine, &simulation->philos[i]) != 0)
            destroy_mutexes(simulation, forks, "Failed to create philosopher thread");
        i++;
    }
	printf("Created philosopher threads\n");
	if (pthread_create(&simulation->monitor, NULL, monitor, simulation) != 0)
        destroy_mutexes(simulation, forks, "Failed to create monitor thread");
	printf("Created monitor thread\n");
    while (i < simulation->philos[0].num_of_philos)
	{
        if (pthread_join(simulation->philos[i].thread, NULL) != 0)
            destroy_mutexes(simulation, forks, "Failed to join philosopher thread");
		i++;
    }
	printf("Joined philosopher threads\n");
	if (pthread_join(simulation->monitor, NULL) != 0)
        destroy_mutexes(simulation, forks, "Failed to join monitor thread");
	printf("Joined monitor thread\n");
}


void	init_philos(t_philo *philos, t_sim *simulation, pthread_mutex_t *forks, char **argv)
{
	int i;
	
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philos[i].num_of_philos = ft_atoi(argv[1]);
		philos[i].time_to_die = ft_atoi(argv[2]);
		philos[i].time_to_eat = ft_atoi(argv[3]);
		philos[i].time_to_sleep = ft_atoi(argv[4]);
		if (argv[5])
			philos[i].times_must_eat = ft_atoi(argv[5]);
		else
			philos[i].times_must_eat = -1;
		philos[i].id = i + 1;
		philos[i].start_time = get_time();
		philos[i].last_meal = get_time();
		philos[i].eating = 0;
		philos[i].eat_count = 0;
		philos[i].dead = &simulation->dead_flag;
		philos[i].left_fork = &forks[i];
		if (i == 0)
			philos[i].right_fork = &forks[philos[i].num_of_philos - 1];
		else
			philos[i].right_fork = &forks[i - 1];
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
	init_simulation(&simulation, philos);
	init_forks(forks, ft_atoi(argv[1])); // init forks
	init_philos(philos, &simulation, forks, argv); // init philo structs
	create_threads(&simulation, forks); // create threads, run loop
	destroy_mutexes(&simulation, forks, NULL); // destroy mutexes
	return (0);
}