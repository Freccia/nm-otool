/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:31:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/12 18:59:18 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM
# define FT_NM

# include "libft.h"
# include "ft_common.h"

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>

# define RADR	"radr://"

typedef struct s_strtab		t_strtab;
typedef struct s_sections	t_sections;

struct s_sections
{
	uint16_t		st_text;
	uint32_t		st_data;
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

int			nm_handle_fat(void *ptr);
int			nm_handle_64(void *ptr);
int			nm_handle_32(void *ptr);

/*
**	SECTIONS
*/
void		fill_sections_64(t_sections *sects, struct load_command *lc, uint32_t ncmds);
void		fill_sections_32(t_sections *sects, struct load_command *lc, uint32_t ncmds);

/*
**	LIST
*/
int			list_push(t_strtab **slist, struct nlist_64 *symtab, char *strtab);
void		insertion_sort(t_strtab **slist);
void		free_list(t_strtab *slist);

/*
**	TOOLS
*/
char		get_symbol_type(uint8_t n_type, uint16_t nsects, t_sections sects);
void		print_list_32(t_strtab *slist, t_sections sects);
void		print_list_64(t_strtab *slist, t_sections sects);

#endif
