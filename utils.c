/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yihakan <yihakan@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:43:06 by yihakan           #+#    #+#             */
/*   Updated: 2025/08/28 00:45:24 by yihakan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

long ft_get_time_ms(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void ft_usleep_ms(long milliseconds)
{
    long start_time;

    start_time = ft_get_time_ms();
    while ((ft_get_time_ms() - start_time) < milliseconds)
        usleep(500);
}

void ft_print_status(t_philosopher *p_philo, const char *status, const char *color)
{
    long            timestamp;
    t_program_data  *p_data;

    p_data = p_philo->t_program_data;
    pthread_mutex_lock(&p_data->t_write_mutex);
    if (p_data->t_simulation_running)
    {
        timestamp = ft_get_time_ms() - p_data->t_start_time;
        printf("%s%ld %d %s\n" RESET, color, timestamp, p_philo->t_id, status);
    }
    pthread_mutex_unlock(&p_data->t_write_mutex);
}


