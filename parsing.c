/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yihakan <yihakan@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:42:41 by yihakan           #+#    #+#             */
/*   Updated: 2025/08/28 01:35:18 by yihakan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

int	ft_is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_is_all_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_is_digit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

long	ft_atol(const char *str)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_is_digit(*str))
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

int	ft_parse_args(int argc, char **argv, t_program_data *p_data)
{
	if (argc != 5 && argc != 6)
		return (ft_error("Invalid number of arguments."));
	if (!ft_is_all_digits(argv[1]) || !ft_is_all_digits(argv[2]) || \
		!ft_is_all_digits(argv[3]) || !ft_is_all_digits(argv[4]))
		return (ft_error("Arguments must be positive numbers."));
	p_data->t_num_philosophers = ft_atol(argv[1]);
	p_data->t_time_to_die = ft_atol(argv[2]);
	p_data->t_time_to_eat = ft_atol(argv[3]);
	p_data->t_time_to_sleep = ft_atol(argv[4]);
	if (p_data->t_num_philosophers <= 0 || p_data->t_time_to_die < 0 || \
		p_data->t_time_to_eat < 0 || p_data->t_time_to_sleep < 0)
		return (ft_error("Arguments must be positive numbers."));
	if (argc == 6)
	{
		if (!ft_is_all_digits(argv[5]))
			return (ft_error("Arguments must be positive numbers."));
		p_data->t_num_times_to_eat = ft_atol(argv[5]);
		if (p_data->t_num_times_to_eat < 0)
			return (ft_error("Arguments must be positive numbers."));
	}
	else
		p_data->t_num_times_to_eat = -1;
	return (0);
}
