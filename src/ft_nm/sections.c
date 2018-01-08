/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/08 14:41:03 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/08 14:52:52 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static uint32_t	init_section_64(t_sections *sects, uint32_t nsects, 
		struct section_64 *section, uint32_t j)
{
	uint32_t	k;

	k = 1;
	while (k <= nsects)
	{
		if (!ft_strcmp(section->sectname, SECT_TEXT))
			sects->st_text = j;
		else if (!ft_strcmp(section->sectname, SECT_DATA))
			sects->st_data = j;
		else if (!ft_strcmp(section->sectname, SECT_BSS))
			sects->st_bss = j;
		else if (!ft_strcmp(section->sectname, SECT_COMMON))
			sects->st_common = j;
		++k;
		++j;
		section = (struct section_64 *)((char*)section + sizeof(struct section_64));
	}
	return (j);
}

void		fill_sections_64(t_sections *sects, struct load_command *lc, uint32_t ncmds)
{
	struct segment_command_64	*segcomm;
	struct section_64			*section;
	uint32_t					i;
	uint32_t					j;

	i = 0;
	j = 1;
	while (i < ncmds)
	{
		segcomm = (struct segment_command_64 *)lc;
		if (lc->cmd == LC_SEGMENT_64 && ft_strcmp(SEG_PAGEZERO, segcomm->segname) != 0)
		{
			section = (struct section_64 *)((uint8_t*)segcomm + sizeof(struct segment_command_64));
			j += init_section_64(sects, segcomm->nsects, section, j);
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
}

static uint32_t	init_section_32(t_sections *sects, uint32_t nsects, 
		struct section *section, uint32_t j)
{
	uint32_t	k;

	k = 1;
	while (k <= nsects)
	{
		if (!ft_strcmp(section->sectname, SECT_TEXT))
			sects->st_text = j;
		else if (!ft_strcmp(section->sectname, SECT_DATA))
			sects->st_data = j;
		else if (!ft_strcmp(section->sectname, SECT_BSS))
			sects->st_bss = j;
		else if (!ft_strcmp(section->sectname, SECT_COMMON))
			sects->st_common = j;
		++k;
		++j;
		section = (struct section *)((char*)section + sizeof(struct section));
	}
	return (j);
}

void		fill_sections_32(t_sections *sects, struct load_command *lc, uint32_t ncmds)
{
	struct segment_command	*segcomm;
	struct section			*section;
	uint32_t					i;
	uint32_t					j;

	i = 0;
	j = 1;
	while (i < ncmds)
	{
		segcomm = (struct segment_command *)lc;
		if (lc->cmd == LC_SEGMENT && ft_strcmp(SEG_PAGEZERO, segcomm->segname) != 0)
		{
			section = (struct section *)((uint8_t*)segcomm + sizeof(struct segment_command));
			j += init_section_32(sects, segcomm->nsects, section, j);
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
}


