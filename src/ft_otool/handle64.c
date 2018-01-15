/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/12 16:05:13 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/15 18:53:43 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		dump_memory(void *ptr, char *name, uint64_t addr, uint64_t size)
{
	uint8_t		*tmp;
	uint64_t	i;
	uint64_t	k;

	ft_printf("%s:\n", name);
	ft_printf("Contents of (__TEXT,__text) section\n");
	tmp = (uint8_t *)ptr;
	i = 0;
	while (i < size)
	{
		k = 0;
		ft_printf("%016lx	", addr + i);
		while (k < 16 && (i + k) < size)
		{
			ft_printf("%02x ", tmp[i + k]);
			++k;
		}
		ft_putchar('\n');
		i += 16;
	}
}

static void		output_64(void *ptr, char *name,
		struct segment_command_64 *segc)
{
	struct section_64		*sect;
	uint32_t				i;

	sect = (struct section_64 *)((uint8_t *)segc +
			sizeof(struct segment_command_64));
	i = 0;
	while (i < segc->nsects)
	{
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
		{
			dump_memory((void *)((uint8_t*)ptr + sect->offset),
					name, sect->addr, sect->size);
		}
		sect = (struct section_64*)((uint8_t*)sect + sizeof(struct section_64));
		++i;
	}
}

void			otool_handle_64(void *ptr, char *name)
{
	struct mach_header_64		*header;
	struct segment_command_64	*segc;
	struct load_command			*lc;
	uint32_t					i;

	header = (struct mach_header_64 *)ptr;
	lc = ptr + sizeof(*header);
	i = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			segc = (struct segment_command_64 *)lc;
			output_64(ptr, name, segc);
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
}
