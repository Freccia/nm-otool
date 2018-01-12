/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/10 20:04:20 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/12 18:15:47 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		parse_symtab_command(size_t size,
		struct symtab_command *symc, size_t tot)
{
	int				i;

	if (symc->stroff > size)
		return (EXIT_FAILURE);
	i = 0;
	while (i < (int)symc->nsyms)
	{
		tot += sizeof(struct nlist_64);
		if (tot > size)
			return (EXIT_FAILURE);
		++i;
	}
	return (EXIT_SUCCESS);
}

static int		parse_segment_command_32(size_t size,
		struct segment_command *segc32, size_t tot)
{
	struct section		*sect;
	uint32_t			k;

	sect = (struct section *)((uint8_t *)segc32 + sizeof(*segc32));
	k = 0;
	while (k < segc32->nsects)
	{
		tot += sizeof(struct section);
		if (tot < size)
			return (EXIT_FAILURE);
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
		{
			if ((size_t)sect->offset > size)
				return (EXIT_FAILURE);
		}
		++k;
	}
	return (EXIT_SUCCESS);
}

static int		parse_segment_command_64(size_t size,
		struct segment_command_64 *segc64, size_t tot)
{
	struct section_64	*sect;
	uint32_t			k;

	sect = (struct section_64 *)((uint8_t *)segc64 + sizeof(*segc64));
	k = 0;
	while (k < segc64->nsects)
	{
		tot += sizeof(struct section_64);
		if (tot < size)
			return (EXIT_FAILURE);
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
		{
			if ((size_t)sect->offset > size)
				return (EXIT_FAILURE);
		}
		++k;
	}
	return (EXIT_SUCCESS);
}

static int		parse_load_commands_bis(size_t size,
		struct load_command *lc, size_t tot)
{
		struct segment_command_64	*segc64;
		struct segment_command		*segc32;
		struct symtab_command		*symc;

		if (lc->cmd == LC_SYMTAB)
		{
			symc = (struct symtab_command *)lc;
			if (parse_symtab_command(size, symc, tot))
				return (EXIT_FAILURE);
		}
		else if (lc->cmd == LC_SEGMENT)
		{
			segc32 = (struct segment_command *)lc;
			if (parse_segment_command_32(size, segc32, tot))
				return (EXIT_FAILURE);
		}
		else if (lc->cmd == LC_SEGMENT_64)
		{
			segc64 = (struct segment_command_64 *)lc;
			if (parse_segment_command_64(size, segc64, tot))
				return (EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
}

static int		parse_load_commands(size_t size, uint32_t ncmds,
		struct load_command *lc)
{
	void		*ptr;
	size_t		tot;
	uint32_t	i;

	ptr = NULL;
	tot = 0;
	i = 0;
	while (i < ncmds)
	{
		parse_load_commands_bis(size, lc, tot);
		tot += (size_t)lc->cmdsize;
		if (tot > size)
			return (EXIT_FAILURE);
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}

static int		parse_fat_arch(void *ptr, size_t size, struct fat_arch *arch_ptr)
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

static int		parse_fat(void *ptr, size_t size)
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

int				ft_nm_parse(void *ptr, size_t size)
{
	struct mach_header		*header;
	struct load_command		*lc;

	header = (struct mach_header *)ptr;
	lc = ptr + sizeof(*header);
	if (header->magic == FAT_MAGIC || header->magic == FAT_CIGAM)
		return (parse_fat(ptr, size));
	if (parse_load_commands(size, header->ncmds, lc))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
