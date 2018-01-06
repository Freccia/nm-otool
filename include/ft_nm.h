/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:31:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/06 22:13:38 by lfabbro          ###   ########.fr       */
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

struct s_sections
{
	uint16_t		sct_text;
	uint16_t		sct_data;
	uint16_t		sct_
};

struct s_strtab
{
	t_strtab		*next;
	char			*strx;
	uint8_t			type;
	uint16_t		sect;
	uint16_t		desc;
	uint64_t		value;
};

int			handle_64(void *ptr);
int			handle_32(void *ptr);

int			list_push(t_strtab **slist, struct nlist_64 symtab, char *strtab);
void		insertion_sort(t_strtab **slist);

int			error(char *str);

#endif
