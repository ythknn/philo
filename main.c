/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yihakan <yihakan@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:41:28 by yihakan           #+#    #+#             */
/*   Updated: 2025/08/28 00:45:24 by yihakan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

static void ft_cleanup(t_program_data *p_data)
{
    int i;

    i = 0;
    while (i < p_data->t_num_philosophers)
    {
        pthread_mutex_destroy(&p_data->t_forks[i]);
        i++;
    }
    pthread_mutex_destroy(&p_data->t_write_mutex);
    pthread_mutex_destroy(&p_data->t_meal_check_mutex);
    pthread_mutex_destroy(&p_data->t_death_mutex);
    free(p_data->t_forks);
    free(p_data->t_philosophers);
}

int main(int argc, char **argv)
{
    t_program_data  p_data;
    int             i;

    if (ft_parse_args(argc, argv, &p_data) != 0)
        return (1);
    if (ft_init_data(&p_data) != 0)
        return (1);

    i = 0;
    while (i < p_data.t_num_philosophers)
    {
        if (pthread_create(&p_data.t_philosophers[i].t_thread, NULL, \
            ft_philo_routine, &p_data.t_philosophers[i]) != 0)
            return (ft_error("Failed to create philosopher thread."));
        i++;
    }

    if (pthread_create(&p_data.t_monitor_thread, NULL, \
        ft_monitor_routine, &p_data) != 0)
        return (ft_error("Failed to create monitor thread."));

    i = 0;
    while (i < p_data.t_num_philosophers)
    {
        if (pthread_join(p_data.t_philosophers[i].t_thread, NULL) != 0)
            return (ft_error("Failed to join philosopher thread."));
        i++;
    }
    if (pthread_join(p_data.t_monitor_thread, NULL) != 0)
        return (ft_error("Failed to join monitor thread."));

    ft_cleanup(&p_data);
    return (0);
}


