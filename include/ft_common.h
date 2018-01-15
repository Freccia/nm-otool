/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_common.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/12 18:51:54 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/15 17:47:09 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_COMMON
# define FT_COMMON

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <ar.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>

# define FT_ARMAG				(uint32_t)0x0a3e686372613c21
# define ARMAG_LEN				(uint8_t)SARMAG
# define ARCHIVE_HEADER_SIZE	(uint32_t)0x3c

# define SUPPORTED_ARCH(x)	x == MH_MAGIC_64 || x == MH_CIGAM_64 || \
							   x == MH_MAGIC || x == MH_CIGAM

# define SUPPORTED_FAT(x)	x == FAT_MAGIC_64 || x == FAT_CIGAM_64 || \
							   x == FAT_MAGIC || x == FAT_CIGAM

# define SWAP_SHORT(a) ( ((a & 0xff) << 8) | ((unsigned short)(a) >> 8) )

# define SWAP_INT(a)  ( ((a) << 24) | \
		(((a) << 8) & 0x00ff0000) | \
		(((a) >> 8) & 0x0000ff00) | \
		((unsigned int)(a) >> 24) )

typedef struct s_archive	t_archive;

struct s_archive
{
	struct ar_hdr	*ar;
	uint32_t		symtab_size;
	uint32_t		symtab_offset;
	uint32_t		*symtab;
};

/*
**	PARSE
*/
int			ft_parse_binary(void *ptr, size_t size);
int			parse_fat(void *ptr, size_t size);
int			parse_archive(void *ptr, size_t size);
int			parse_load_commands(size_t size, uint32_t ncmds,
		struct load_command *lc);

int			get_object_offset(struct ar_hdr *ar);

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
