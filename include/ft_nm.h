/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:31:12 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/08 21:24:54 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM
# define FT_NM

# include "libft.h"

# include <stdio.h>  //norme
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>

# define RANLIB 0x0a3e686372613c21
# define RADR	"radr://"

enum byte_sex {
	UNKNOWN_BYTE_SEX,
	BIG_ENDIAN_BYTE_SEX,
	LITTLE_ENDIAN_BYTE_SEX
};

#define SWAP_SHORT(a) ( ((a & 0xff) << 8) | ((unsigned short)(a) >> 8) )

#define SWAP_INT(a)  ( ((a) << 24) | \
		(((a) << 8) & 0x00ff0000) | \
		(((a) >> 8) & 0x0000ff00) | \
		((unsigned int)(a) >> 24) )

typedef struct s_strtab		t_strtab;
typedef struct s_sections	t_sections;
typedef struct s_cnt		cnt;

struct s_cnt
{
	uint8_t			i;
	uint8_t			j;
	uint8_t			k;
};

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

int			handle_fat(void *ptr);
int			handle_64(void *ptr);
int			handle_32(void *ptr);


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

/*
**	SWAP
*/
int			should_swap_bytes_fat(uint32_t magic);
int			should_swap_bytes(uint32_t magic);
void		swap_fat_header(struct fat_header *fat_header);
void		swap_fat_arch(struct fat_arch *fat_archs, unsigned long nfat_arch);
void		swap_fat(struct fat_header *fat_header, struct fat_arch *fat_archs);

int			error(char *str);

#endif
