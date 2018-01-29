/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:31:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/29 17:58:06 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include "ft_common.h"

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>

# define RADR	"radr://"

# define OPT_G		1
# define OPT_UL		2
# define OPT_UU		4
# define OPT_X		8
# define OPT_J		16

typedef struct s_options	t_options;
typedef struct s_strtab		t_strtab;
typedef struct s_sections	t_sections;
typedef struct s_quad		t_quad;

struct		s_quad
{
	uint32_t		i;
	uint32_t		tot;
	uint32_t		off;
	uint32_t		tmp;
};

struct		s_options
{
	uint32_t		opt;
	t_quad			q;
};

struct		s_sections
{
	uint16_t		st_text;
	uint32_t		st_data;
	uint16_t		st_bss;
	uint16_t		st_common;
};

struct		s_strtab
{
	t_strtab		*next;
	char			*strx;
	uint8_t			type;
	uint16_t		sect;
	uint16_t		desc;
	uint64_t		value;
	uint64_t		nstrx;
};

int			nm_handle_32(void *ptr, t_options opt);
int			nm_handle_64(void *ptr, t_options opt);
int			nm_handle_fat(void *ptr, t_options opt);
int			nm_handle_archive(void *ptr, t_options opt, char *name);

/*
**	SECTIONS
*/
void		fill_sections_64(t_sections *sects, struct load_command *lc,
		uint32_t ncmds);
void		fill_sections_32(t_sections *sects, struct load_command *lc,
		uint32_t ncmds);

/*
**	LIST
*/
int			list_push(t_strtab **slist, struct nlist_64 *symtab, char *strtab);
void		insertion_sort(t_strtab **slist);
void		free_list(t_strtab *slist);

/*
**	OPTIONS
*/
int			get_options(int ac, char **av, t_options *opt);

/*
**	TOOLS
*/
void		print_list_32(t_strtab *slist, t_sections sects, t_options opt);
void		print_list_64(t_strtab *slist, t_sections sects, t_options opt);

#endif
