/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/12 16:05:13 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/12 19:00:22 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		dump_memory(void *ptr, char *name, uint32_t addr, uint32_t size)
{
	uint8_t		*tmp;
	uint32_t	i;
	uint32_t	k;

	ft_printf("%s:\n", name);
	ft_printf("Contents of (__TEXT,__text) section\n");
	tmp = (uint8_t *)ptr;
	i = 0;
	while (i < size)
	{
		k = 0;
		ft_printf("%08lx	", addr + i);
		while (k < 16 && (i + k) < size)
		{
			ft_printf("%02x ", tmp[i + k]);
			++k;
		}
		ft_putchar('\n');
		i += 16;
	}
}

static void		output_32(void *ptr, char *name, struct segment_command *segc)
{
	struct section	*sect;
	uint32_t		i;

	sect = (struct section *)((uint8_t *)segc +
			sizeof(struct segment_command));
	i = 0;
	while (i < segc->nsects)
	{
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
		{
			dump_memory((void *)((uint8_t *)ptr + sect->offset),
					name, sect->addr, sect->size);
		}
		sect = (struct section *)((uint8_t *)sect + sizeof(struct section));
		++i;
	}
}

void			otool_handle_32(void *ptr, char *name)
{
	struct mach_header			*header;
	struct segment_command		*segc;
	struct load_command			*lc;
	uint32_t					i;

	header = (struct mach_header *)ptr;
	lc = ptr + sizeof(*header);
	i = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			segc = (struct segment_command *)lc;
			if (ft_strcmp(segc->segname, SEG_TEXT) == 0)
			{
				output_32(ptr, name, segc);
				break ;
			}
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
}
