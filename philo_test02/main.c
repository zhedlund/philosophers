#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define PHILO_MAX 50

typedef struct {
    pthread_mutex_t print_lock;
    pthread_mutex_t meal_lock;
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    unsigned int program_start_time;
} t_program;

typedef struct {
    int philosopher_id;
    int eat_count;
    t_program *program; 
    pthread_mutex_t *fork_left;
    pthread_mutex_t *fork_right;
    pthread_t thread;
} t_philo;

unsigned int get_time(void) {
    struct timeval time;
    if (gettimeofday(&time, NULL) == -1)
        fprintf(stderr, "Error: gettimeofday\n");
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void* philosopher_thread(void* arg) {
    t_philo *philosopher = (t_philo*)arg;
    t_program *program = philosopher->program;

    struct timeval last_meal_time;
    gettimeofday(&last_meal_time, NULL);

    while (1) {
        printf("%d is thinking\n", philosopher->philosopher_id);
        usleep(100);

        pthread_mutex_lock(&program->meal_lock); // meal lock
		pthread_mutex_lock(philosopher->fork_left);
        printf("%d has taken a fork\n", philosopher->philosopher_id);
        pthread_mutex_lock(philosopher->fork_right);
        printf("%d has taken a fork\n", philosopher->philosopher_id);
        printf("%d is eating\n", philosopher->philosopher_id);
		pthread_mutex_unlock(&program->meal_lock);
		
        gettimeofday(&last_meal_time, NULL);
        usleep(program->time_to_eat * 1000);

		pthread_mutex_unlock(philosopher->fork_left);
        pthread_mutex_unlock(philosopher->fork_right);
		
        

        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        long long elapsed_time = (current_time.tv_sec - last_meal_time.tv_sec) * 1000LL +
                                 (current_time.tv_usec - last_meal_time.tv_usec) / 1000;
        if (elapsed_time > program->time_to_die) {
            printf("Philosopher %d died\n", philosopher->philosopher_id);
            // Handle philosopher death
        }

        
        usleep(program->time_to_sleep * 1000);
    }

    return NULL;
}

void initialize(t_program *program, t_philo philos[], int number_of_philosophers, pthread_mutex_t forks[]) {
    pthread_mutex_init(&program->print_lock, NULL);
    pthread_mutex_init(&program->meal_lock, NULL); // Initialize meal lock
    program->number_of_philosophers = number_of_philosophers;

    for (int i = 0; i < number_of_philosophers; i++) {
        philos[i].philosopher_id = i;
        philos[i].eat_count = 0;
        philos[i].program = program;
        philos[i].fork_left = &forks[i];
        philos[i].fork_right = &forks[(i + 1) % number_of_philosophers];
        pthread_mutex_init(&forks[i], NULL);
    }
}

void create_threads(t_philo philos[], int number_of_philosophers) {
    for (int i = 0; i < number_of_philosophers; i++) {
        pthread_create(&philos[i].thread, NULL, philosopher_thread, (void *)&philos[i]);
    }
}

int main(int argc, char *argv[]) {
    t_program program;
    t_philo philos[PHILO_MAX];
    pthread_mutex_t forks[PHILO_MAX];

    program.number_of_philosophers = atoi(argv[1]);
    program.time_to_die = atoi(argv[2]);
    program.time_to_eat = atoi(argv[3]);
    program.time_to_sleep = atoi(argv[4]);
    program.number_of_times_each_philosopher_must_eat = (argc == 6) ? atoi(argv[5]) : -1;
    program.program_start_time = get_time();
    pthread_mutex_init(&program.print_lock, NULL);
	pthread_mutex_init(&program.meal_lock, NULL); // Initialize meal lock

    initialize(&program, philos, program.number_of_philosophers, forks);

    create_threads(philos, program.number_of_philosophers);

    for (int i = 0; i < program.number_of_philosophers; i++) {
        pthread_join(philos[i].thread, NULL);
    }

    pthread_mutex_destroy(&program.print_lock);
    pthread_mutex_destroy(&program.meal_lock); // Destroy meal lock
    for (int i = 0; i < PHILO_MAX; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    return 0;
}
