/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/07 23:52:33 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/08 15:49:17 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int			should_swap_bytes_fat(uint32_t magic)
{
	return (magic == FAT_CIGAM || magic == FAT_CIGAM_64);
}

int			should_swap_bytes(uint32_t magic)
{
	return (magic == MH_CIGAM || magic == MH_CIGAM_64);
}

void swap_fat_header(struct fat_header *fat_header)
{
	fat_header->nfat_arch = SWAP_INT(fat_header->nfat_arch);
	fat_header->magic = SWAP_INT(fat_header->magic);
}

void swap_fat_arch(struct fat_arch *fat_archs, unsigned long nfat_arch)
{
    unsigned long i;

	i = 0;
	while (i < nfat_arch)
	{
	    fat_archs[i].cputype = SWAP_INT(fat_archs[i].cputype);
	    fat_archs[i].cpusubtype = SWAP_INT(fat_archs[i].cpusubtype);
	    fat_archs[i].offset = SWAP_INT(fat_archs[i].offset);
	    fat_archs[i].size = SWAP_INT(fat_archs[i].size);
	    fat_archs[i].align = SWAP_INT(fat_archs[i].align);
		++i;
	}
}

void	swap_fat(struct fat_header *header, struct fat_arch *fat_archs)
{
	swap_fat_header(header);
	swap_fat_arch(fat_archs, header->nfat_arch);
}
