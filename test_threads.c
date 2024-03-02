/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 21:29:42 by zhedlund          #+#    #+#             */
/*   Updated: 2024/03/02 16:17:08 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // printf
#include <stdlib.h> //malloc
#include <pthread.h>
#include <unistd.h>

void    exit_on_error(int *num, int exit_code)
{
    free(num);
    exit(exit_code);
}

void    *routine_2(void  *arg)
{
    int *num;
    int i;

    num = (int *)arg;
    i = 0;
    while (i < 1000000)
    {
        (*num)++;
        i++;
    }
    return (NULL);
}

void    example_3()
{
    int *num;
    pthread_t   t1;
    pthread_t   t2;

    num = malloc(sizeof(int));
    if (num == NULL)
        exit(1);
    *num = 0;
    if (pthread_create(&t1, NULL, routine_2, num))
        exit_on_error(num, 2);
    if (pthread_create(&t2, NULL, routine_2, num))ft_usleep(1);
        exit_on_error(num, 2);
    if (pthread_join(t1, NULL))
        exit_on_error(num, 3);
    if (pthread_join(t2, NULL))
        exit_on_error(num, 3);
    printf("Num: %d\n", *num);
}

void    *print_01(void   *arg)
{
    int i;

    i = 0;
    while (i < 100)
    {
        printf("Test 1\n");
        i++;
        usleep(100);
    }
    return (NULL);
}

void    *print_02(void   *arg)
{
    int i;

    i = 0;
    while (i < 100)
    {
        printf("\t\tTest 2\n");
        i++;
        usleep(100);
    }
    return (NULL);
}

void    example_2()
{
    pthread_t   t1;
    pthread_t   t2;

    if (pthread_create(&t1, NULL, print_01, NULL))
        exit(1);
    if (pthread_create(&t2, NULL, print_02, NULL))
        exit(1);
    if (pthread_join(t1, NULL))
        exit(1);
    if (pthread_join(t2, NULL))
        exit(1);

}

void    example_1()
{
    print_01(NULL);
    print_02(NULL);
}

int main()
{
    example_1();
	example_2();
	example_3();
	
    return (0);
}