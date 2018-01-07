/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:31:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/07 16:58:35 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM
# define FT_NM

# include "libft.h"

# include <stdio.h>  //norme
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <fcntl.h>
# include <sys/stat.h>

# define RADR	"radr://"

typedef struct s_strtab		t_strtab;
typedef struct s_sections	t_sections;

struct s_sections
{
	uint16_t		st_text;
	uint16_t		st_data;
	uint16_t		st_bss;
	uint16_t		st_common;
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
void		free_list(t_strtab *slist);

int			error(char *str);

#endif
