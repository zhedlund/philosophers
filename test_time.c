/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_time.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 21:40:24 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/01 16:08:17 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // printf
#include <sys/time.h> // time
#include <stdint.h> // uint64_t
#include <unistd.h> // usleep

// struct timeval {
//     time_t      tv_sec;     // seconds
//     suseconds_t tv_usec;    // microseconds
// };

// int gettimeofday(struct timeval *tv, struct timezone *tz);
int		get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_usleep(unsigned int sleep_time)
{
	unsigned int	start;

	start = get_time();
	while ((get_time() - start) < sleep_time)
		usleep(500);
}

void    example_1()
{
    struct timeval time;

    gettimeofday(&time, NULL);
    printf("%ld seconds\n", time.tv_sec);
    printf("%ld macroseconds\n", time.tv_usec);

    printf("\n%ld years passed since 1970\n", time.tv_sec / 60 / 60 / 24 / 365);
}

void    example_2()
{
    uint64_t    start_time;
    uint64_t    now;

    start_time = get_time();
    ft_usleep(50); // accepts in microseconds
    now = get_time();
    printf("%ld milliseconds passed since the start\n", now - start_time);
}

int main()
{
	struct timeval	time;
    
	example_1();
    example_2();

    return (0);
}