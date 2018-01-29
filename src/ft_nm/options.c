/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:04:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/23 15:53:21 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int	is_opt(char *arg)
{
		if (arg && ft_strlen(arg) &&
						arg[0] == '-')
				return (1);
		return (0);
}

int			get_options(int ac, char **av, t_options *opt)
{
		int		i;
		int		k;

		i = 1;
		while (i < ac)
		{
				if (!is_opt(av[i]))
						break ;
				k = 0;
				while (av[i][k])
				{
						if (k == 0 && av[i][k] == '-')
								;
						else if (av[i][k] == 'g')
								opt->opt += OPT_G;
						else if (av[i][k] == 'u')
								opt->opt += OPT_UL;
						else if (av[i][k] == 'U')
								opt->opt += OPT_UU;
						else if (av[i][k] == 'x')
								opt->opt += OPT_X;
						else if (av[i][k] == 'j')
								opt->opt += OPT_J;
						else
								return (-1);
						++k;
				}
				++i;
		}
		return (i);
}
