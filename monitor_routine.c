/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yihakan <yihakan@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:42:33 by yihakan           #+#    #+#             */
/*   Updated: 2025/08/28 01:30:09 by yihakan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

static int ft_is_philo_dead(t_philosopher *p_philo, t_program_data *p_data)
{
    long    time_since_last_meal;

    pthread_mutex_lock(&p_data->t_meal_check_mutex);
    time_since_last_meal = ft_get_time_ms() - p_philo->t_last_meal_time;
    pthread_mutex_unlock(&p_data->t_meal_check_mutex);
    if (time_since_last_meal > p_data->t_time_to_die)
    {
        pthread_mutex_lock(&p_data->t_death_mutex);
        pthread_mutex_lock(&p_data->t_write_mutex);
        p_data->t_simulation_running = 0;
        printf("%s%ld %d died\n" RESET, RED, ft_get_time_ms() - p_data->t_start_time, p_philo->t_id);
        pthread_mutex_unlock(&p_data->t_write_mutex);
        pthread_mutex_unlock(&p_data->t_death_mutex);
        return (1);
    }
    return (0);
}

static int ft_all_philos_eaten(t_program_data *p_data)
{
    int i;

    if (p_data->t_num_times_to_eat == -1)
        return (0);
    i = 0;
    pthread_mutex_lock(&p_data->t_meal_check_mutex);
    while (i < p_data->t_num_philosophers)
    {
        if (p_data->t_philosophers[i].t_meals_eaten < p_data->t_num_times_to_eat)
        {
            pthread_mutex_unlock(&p_data->t_meal_check_mutex);
            return (0);
        }
        i++;
    }
    pthread_mutex_unlock(&p_data->t_meal_check_mutex);
    pthread_mutex_lock(&p_data->t_death_mutex);
    p_data->t_simulation_running = 0;
    pthread_mutex_unlock(&p_data->t_death_mutex);
    return (1);
}

void *ft_monitor_routine(void *arg)
{
    t_program_data  *p_data;
    int             i;

    p_data = (t_program_data *)arg;
    while (1)
    {
        i = 0;
        while (i < p_data->t_num_philosophers)
        {
            if (ft_is_philo_dead(&p_data->t_philosophers[i], p_data))
                return (NULL);
            i++;
        }
        if (ft_all_philos_eaten(p_data))
            return (NULL);
        pthread_mutex_lock(&p_data->t_death_mutex);
        if (!p_data->t_simulation_running)
        {
            pthread_mutex_unlock(&p_data->t_death_mutex);
            break;
        }
        pthread_mutex_unlock(&p_data->t_death_mutex);
        usleep(100);
    }
    return (NULL);
}


