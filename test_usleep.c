/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_usleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 21:41:55 by zhedlund          #+#    #+#             */
/*   Updated: 2024/02/29 21:50:12 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // printf
#include <sys/time.h> // time
#include <unistd.h> // usleep

// struct timeval {
//     time_t      tv_sec;     // seconds
//     suseconds_t tv_usec;    // microseconds
// };

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

void    example()
{
    struct timeval  start_time;
    struct timeval  end_time;
    long requested_sleep_time;
    long actual_sleep_time;

    requested_sleep_time = 200 * 1000;  // 200 milliseconds
    gettimeofday(&start_time, NULL);
    usleep(requested_sleep_time);
   	gettimeofday(&end_time, NULL);
    actual_sleep_time = (end_time.tv_sec - start_time.tv_sec) * 100000 + (end_time.tv_usec - start_time.tv_usec);

    printf("Requested Sleep Time: %ld microseconds\n", requested_sleep_time);
    printf("Actual Sleep Time: %ld microseconds\n", actual_sleep_time);
}

int main()
{
    example();
	
    return (0);
}