/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yihakan <yihakan@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:42:14 by yihakan           #+#    #+#             */
/*   Updated: 2025/08/28 01:28:35 by yihakan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

int ft_error(const char *msg)
{
    printf(RED "Error: %s\n" RESET, msg);
    return (1);
}
