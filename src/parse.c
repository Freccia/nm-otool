/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/10 20:04:20 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/15 17:12:55 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int	parse_fat_arch(void *ptr, size_t size, struct fat_arch *arch_ptr)
{
	struct mach_header	*mach_header;
	struct load_command *lc;

	mach_header = (void *)ptr + arch_ptr->offset;
	lc = (void*)mach_header + sizeof(*mach_header);
	if (arch_ptr->cputype == CPU_TYPE_I386 &&
			parse_load_commands(size, mach_header->ncmds, lc))
		return (EXIT_FAILURE);
	if (arch_ptr->cputype == CPU_TYPE_X86_64 &&
			parse_load_commands(size, mach_header->ncmds, lc))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int			parse_fat(void *ptr, size_t size)
{
	struct fat_header	*header;
	struct fat_arch		*arch_ptr;
	size_t				tot_off;
	size_t				tot;
	uint32_t			i;

	header = (struct fat_header *)ptr;
	arch_ptr = (struct fat_arch *)((char *)header + sizeof(*header));
	tot_off = 0;
	tot = 0;
	i = 0;
	if (should_swap_bytes_fat(header->magic))
		swap_fat(header, arch_ptr);
	while (i++ < header->nfat_arch)
	{
		tot_off += sizeof(arch_ptr->offset);
		if (tot_off > size)
			return (EXIT_FAILURE);
		if (parse_fat_arch(ptr, size, arch_ptr))
			return (EXIT_FAILURE);
		tot += sizeof(*arch_ptr);
		if (tot > size)
			return (EXIT_FAILURE);
		arch_ptr = (struct fat_arch *)((char *)arch_ptr + sizeof(*arch_ptr));
	}
	return (EXIT_SUCCESS);
}

int			ft_parse_binary(void *ptr, size_t size)
{
	struct mach_header		*header;
	struct load_command		*lc;

	header = (struct mach_header *)ptr;
	lc = ptr + sizeof(*header);
	if (SUPPORTED_FAT(header->magic))
		return (parse_fat(ptr, size));
	if (SUPPORTED_ARCH(header->magic))
		return (parse_load_commands(size, header->ncmds, lc));
	else if (header->magic == FT_ARMAG)
		return (parse_archive(ptr, size));
	else
	{
		ft_printf("Binary file not supported.\n");
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
