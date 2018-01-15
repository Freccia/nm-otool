/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_common.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/12 18:51:54 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/15 13:25:36 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_COMMON
# define FT_COMMON

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>

/*
**	RANLIB = "!<arch>\n"  in little endian
**	RADR is a debugging symbol, not to be printed
*/

# define RANLIB (uint32_t)0x0a3e686372613c21

# define SUPPORTED_ARCH(x)	x == MH_MAGIC_64 || x == MH_CIGAM_64 || \
							  x == MH_MAGIC || x == MH_CIGAM

# define SUPPORTED_FAT(x)	x == FAT_MAGIC_64 || x == FAT_CIGAM_64 || \
							   x == FAT_MAGIC || x == FAT_CIGAM

# define SWAP_SHORT(a) ( ((a & 0xff) << 8) | ((unsigned short)(a) >> 8) )

# define SWAP_INT(a)  ( ((a) << 24) | \
		(((a) << 8) & 0x00ff0000) | \
		(((a) >> 8) & 0x0000ff00) | \
		((unsigned int)(a) >> 24) )

/*
**	PARSE
*/
int			ft_parse_binary(void *ptr, size_t size);
int			parse_load_commands(size_t size, uint32_t ncmds,
		struct load_command *lc);

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
