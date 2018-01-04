/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:31:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/04 19:23:40 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM
# define FT_NM

#include "libft.h"

#include <stdio.h>  //norme
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct s_strtab		t_strtab;

struct s_strtab
{
	t_strtab		*next;
	char			*symbol;
};

int			handle_64(void *ptr);
int			handle_32(void *ptr);

int			error(char *str);

#endif
