/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yihakan <yihakan@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:42:50 by yihakan           #+#    #+#             */
/*   Updated: 2025/08/28 01:26:33 by yihakan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

void ft_eat(t_philosopher *p_philo)
{
    t_program_data  *p_data;

    p_data = p_philo->t_program_data;
    if (p_data->t_num_philosophers == 1)
    {
        pthread_mutex_lock(p_philo->t_left_fork);
        ft_print_status(p_philo, "has taken a fork", YELLOW);
        while (1)
        {
            pthread_mutex_lock(&p_data->t_death_mutex);
            if (!p_data->t_simulation_running)
            {
                pthread_mutex_unlock(&p_data->t_death_mutex);
                break;
            }
            pthread_mutex_unlock(&p_data->t_death_mutex);
            ft_usleep_ms(1);
        }
        pthread_mutex_unlock(p_philo->t_left_fork);
        return;
    }
    pthread_mutex_t *first_fork, *second_fork;
    if (p_philo->t_left_fork < p_philo->t_right_fork)
    {
        first_fork = p_philo->t_left_fork;
        second_fork = p_philo->t_right_fork;
    }
    else
    {
        first_fork = p_philo->t_right_fork;
        second_fork = p_philo->t_left_fork;
    }
    pthread_mutex_lock(first_fork);
    ft_print_status(p_philo, "has taken a fork", YELLOW);
    pthread_mutex_lock(second_fork);
    ft_print_status(p_philo, "has taken a fork", YELLOW);
    pthread_mutex_lock(&p_data->t_meal_check_mutex);
    p_philo->t_last_meal_time = ft_get_time_ms();
    ft_print_status(p_philo, "is eating", GREEN);
    p_philo->t_meals_eaten++;
    pthread_mutex_unlock(&p_data->t_meal_check_mutex);
    ft_usleep_ms(p_data->t_time_to_eat);
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}

void ft_sleep(t_philosopher *p_philo)
{
    ft_print_status(p_philo, "is sleeping", BLUE);
    ft_usleep_ms(p_philo->t_program_data->t_time_to_sleep);
}

void ft_think(t_philosopher *p_philo)
{
    ft_print_status(p_philo, "is thinking", CYAN);
}

void *ft_philo_routine(void *arg)
{
    t_philosopher   *p_philo;
    t_program_data  *p_data;

    p_philo = (t_philosopher *)arg;
    p_data = p_philo->t_program_data;

    if (p_philo->t_id % 2 == 0)
        ft_usleep_ms(p_data->t_time_to_eat / 2);

    while (1)
    {
        pthread_mutex_lock(&p_data->t_death_mutex);
        if (!p_data->t_simulation_running)
        {
            pthread_mutex_unlock(&p_data->t_death_mutex);
            break;
        }
        pthread_mutex_unlock(&p_data->t_death_mutex);

        ft_eat(p_philo);
        ft_sleep(p_philo);
        ft_think(p_philo);
    }
    return (NULL);
}
