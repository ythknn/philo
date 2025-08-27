/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yihakan <yihakan@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 01:43:17 by yihakan           #+#    #+#             */
/*   Updated: 2025/08/28 01:46:22 by yihakan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"

typedef struct s_program_data
{
	long					t_num_philosophers;
	long					t_time_to_die;
	long					t_time_to_eat;
	long					t_time_to_sleep;
	long					t_num_times_to_eat;
	long					t_start_time;
	int						t_simulation_running;
	pthread_mutex_t			t_write_mutex;
	pthread_mutex_t			t_meal_check_mutex;
	pthread_mutex_t			t_death_mutex;
	pthread_t				t_monitor_thread;
	struct s_philosopher	*t_philosophers;
	pthread_mutex_t			*t_forks;
}	t_program_data;

typedef struct s_philosopher
{
	int				t_id;
	long			t_last_meal_time;
	int				t_meals_eaten;
	pthread_t		t_thread;
	pthread_mutex_t	*t_left_fork;
	pthread_mutex_t	*t_right_fork;
	t_program_data	*t_program_data;
}	t_philosopher;

// main.c
int		main(int argc, char **argv);

// parsing.c
int		ft_parse_args(int argc, char **argv, t_program_data *p_data);
long	ft_atol(const char *str);
int		ft_is_digit(char c);
int		ft_is_all_digits(char *str);

// init.c
int		ft_init_data(t_program_data *p_data);
int		ft_init_philosophers(t_program_data *p_data);
int		ft_init_forks(t_program_data *p_data);

// philo_routine.c
void	*ft_philo_routine(void *arg);
void	ft_eat(t_philosopher *p_philo);
void	ft_sleep(t_philosopher *p_philo);
void	ft_think(t_philosopher *p_philo);

// monitor_routine.c
void	*ft_monitor_routine(void *arg);
int		ft_check_death(t_program_data *p_data);
int		ft_check_all_eaten(t_program_data *p_data);

// utils.c
long	ft_get_time_ms(void);
void	ft_usleep_ms(long ms);
void	ft_print_status(t_philosopher *p_philo, const char *status,
			const char *color);

// error.c
int		ft_error(const char *msg);

#endif
