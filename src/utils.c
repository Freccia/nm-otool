/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:04:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/23 15:53:21 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_common.h"

int				error(char *str)
{
	ft_printfd(2, "%s\n", str);
	return (EXIT_FAILURE);
}

int				usage_otool(char *name)
{
	ft_printfd(2, "Usage: %s /path/to/binary\n", name);
	return (EXIT_FAILURE);
}

int				usage_nm(char *name)
{
	ft_printfd(2, "Usage: %s /path/to/binary\n", name);
	ft_printfd(2, "Options are: \n");
	ft_printfd(2, "		-g     Display only global (external) symbols.\n");
	ft_printfd(2, "		-u     Display only undefined symbols.\n");
	ft_printfd(2, "		-U     Don't display undefined symbols.\n");
	ft_printfd(2, "		-j     Just display the symbol names (no value or type).\n");
	return (EXIT_FAILURE);
}
