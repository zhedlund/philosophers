/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:50:49 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/09 21:17:07 by zhedlund         ###   ########.fr       */
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
	pthread_t		thread;
	int				num_of_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				times_must_eat;
	int				id;
	unsigned int	start_time;
	unsigned int	last_meal;
	int				eating;
	int				eat_count;
	int				*dead;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
	pthread_mutex_t	*print_lock;
}					t_philo;

typedef struct s_sim
{
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	print_lock;
	t_philo			*philos;
}					t_sim;

unsigned int	get_time(void);
int				is_valid_input(int argc, char **argv);
void			init_input(t_philo *philo, char **argv);
void			init_simulation(t_sim *simulation, t_philo *philos);
void			init_philos(t_philo *philos, t_sim *simulation, 
					pthread_mutex_t *forks, char **argv);
void			init_forks(pthread_mutex_t *forks, int num_of_philos);
void			create_threads(t_sim *simulation);
void			join_threads(t_sim *simulation, pthread_t monitor_thread);
void			*philo_routine(void *philo);
void			*monitor_routine(void *philos);
void			print_output(t_philo *philo, char *str);
void			philo_thinking(t_philo *philo);
void			philo_sleeping(t_philo *philo);
void			philo_eating(t_philo *philo);
int				philo_dead(t_philo *philo, size_t time_to_die);
void			handle_solo_philo(t_philo *philo);
int				check_if_dead(t_philo *philos);
int				check_if_all_ate(t_philo *philos);
int				dead_flag(t_philo *philo);
void			destroy_mutexes(t_sim *simulation, pthread_mutex_t *forks);

/* utils */
int				ft_usleep(unsigned int sleep_ms);
int				ft_atoi(const char *str);
int				ft_isdigit(int c);
int				ft_strlen(const char *str);
void			ft_putstr_fd(char *s, int fd);

#endif