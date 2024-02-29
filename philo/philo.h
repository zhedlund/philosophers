/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:50:49 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/29 21:26:36 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>

# define MAX_PHILOS 200

typedef struct s_philo
{
	int				id;
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_to_eat;
	int				eating;
	int				eat_count;
	int				start_time;
	int				last_meal;
	int				*dead;
	pthread_t		thread;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
}					t_philo;

typedef struct s_sim
{
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	print_lock;
	t_philo			*philo;
}					t_sim;


int		is_valid_input(int argc, char **argv);
void	init_simulation(t_philo *philo, t_sim **simulation, pthread_mutex_t *forks,
			char **argv);
void	print_philo(t_philo *philo, char *str);
int		create_threads(t_sim *simulation);
void	*philo_routine(void *philo_ptr);
void	*monitor(void *philo_ptr);
int		get_time(void);
void	init_input(t_philo *philo, char **argv);
void	init_philo(t_philo *philo, t_sim **simulation, pthread_mutex_t *forks,
			char **argv);
void	init_forks(pthread_mutex_t *forks, int philo_count);
void	destroy_mutexes(t_sim *simulation);

/* utils */
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
size_t	ft_strlen(const char *str);

#endif