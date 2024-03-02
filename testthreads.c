#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
pthread_mutex_t mutex;

void *routine(void *arg)
{
	//sleep(1);
	int index = *(int *)arg;
	pthread_mutex_lock(&mutex);
	printf("%d ", primes[index]);
	pthread_mutex_unlock(&mutex);
	return (NULL);
	free(arg);
}

int	main(int argc, char **argv)
{
	pthread_t	th[10];
	pthread_mutex_t	mutex;
	int i;

	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < 10; i++)
	{
		int *a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &routine, a) != 0)
		{
			printf("Error creating thread\n");
			return (1);
		}
	}
	for (i = 0; i < 10; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			printf("Error joining thread\n");
			return (1);
		}
	}
	pthread_mutex_destroy(&mutex);
	return (0);
}