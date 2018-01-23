/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/23 16:02:13 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/23 16:22:38 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		error(char *str)
{
	ft_printfd(2, "%s\n", str);
	return (EXIT_FAILURE);
}

int		usage(char *name)
{
	ft_printfd(2, "Usage: %s /path/to/binary\n", name);
	return (EXIT_FAILURE);
}
