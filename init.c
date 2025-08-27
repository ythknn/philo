/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yihakan <yihakan@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:42:22 by yihakan           #+#    #+#             */
/*   Updated: 2025/08/28 01:28:27 by yihakan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

static int ft_init_mutexes(t_program_data *p_data)
{
    int i;

    if (pthread_mutex_init(&p_data->t_write_mutex, NULL) != 0)
        return (ft_error("Failed to initialize write mutex."));
    if (pthread_mutex_init(&p_data->t_meal_check_mutex, NULL) != 0)
        return (ft_error("Failed to initialize meal check mutex."));
    if (pthread_mutex_init(&p_data->t_death_mutex, NULL) != 0)
        return (ft_error("Failed to initialize death mutex."));
    p_data->t_forks = (pthread_mutex_t *)
        malloc(sizeof(pthread_mutex_t) * p_data->t_num_philosophers);
    if (!p_data->t_forks)
        return (ft_error("Failed to allocate memory for forks."));
    i = 0;
    while (i < p_data->t_num_philosophers)
    {
        if (pthread_mutex_init(&p_data->t_forks[i], NULL) != 0)
            return (ft_error("Failed to initialize fork mutex."));
        i++;
    }
    return (0);
}

static void ft_destroy_mutexes(t_program_data *p_data)
{
    int i;

    pthread_mutex_destroy(&p_data->t_write_mutex);
    pthread_mutex_destroy(&p_data->t_meal_check_mutex);
    pthread_mutex_destroy(&p_data->t_death_mutex);
    i = 0;
    while (i < p_data->t_num_philosophers)
    {
        pthread_mutex_destroy(&p_data->t_forks[i]);
        i++;
    }
    free(p_data->t_forks);
}

int ft_init_philosophers(t_program_data *p_data)
{
    int i;

    p_data->t_philosophers = (t_philosopher *)
        malloc(sizeof(t_philosopher) * p_data->t_num_philosophers);
    if (!p_data->t_philosophers)
        return (ft_error("Failed to allocate memory for philosophers."));
    i = 0;
    while (i < p_data->t_num_philosophers)
    {
        p_data->t_philosophers[i].t_id = i + 1;
        p_data->t_philosophers[i].t_last_meal_time = ft_get_time_ms();
        p_data->t_philosophers[i].t_meals_eaten = 0;
        p_data->t_philosophers[i].t_program_data = p_data;
        p_data->t_philosophers[i].t_left_fork = 
            &p_data->t_forks[i];
        p_data->t_philosophers[i].t_right_fork = 
            &p_data->t_forks[(i + 1) % p_data->t_num_philosophers];
        i++;
    }
    return (0);
}

int ft_init_data(t_program_data *p_data)
{
    p_data->t_simulation_running = 1;
    p_data->t_start_time = ft_get_time_ms();
    if (ft_init_mutexes(p_data) != 0)
        return (1);
    if (ft_init_philosophers(p_data) != 0)
    {
        ft_destroy_mutexes(p_data);
        return (1);
    }
    return (0);
}
