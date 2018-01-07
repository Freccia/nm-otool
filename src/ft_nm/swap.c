/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/07 23:52:33 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/08 00:17:47 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void swap_fat_header(struct fat_header *fat_header)
{
	uint32_t	tmp;

	tmp = SWAP_INT(fat_header->nfat_arch);
	fat_header->nfat_arch = tmp;
	tmp = SWAP_INT(fat_header->magic);
	fat_header->magic = tmp;
}

void swap_fat_arch(struct fat_arch *fat_archs, unsigned long nfat_arch)
{
    unsigned long i;

	i = 0;
	while (i < nfat_arch)
	{
	    fat_archs[i].cputype    = SWAP_LONG(fat_archs[i].cputype);
	    fat_archs[i].cpusubtype = SWAP_LONG(fat_archs[i].cpusubtype);
	    fat_archs[i].offset     = SWAP_LONG(fat_archs[i].offset);
	    fat_archs[i].size       = SWAP_LONG(fat_archs[i].size);
	    fat_archs[i].align      = SWAP_LONG(fat_archs[i].align);
		++i;
	}
}
