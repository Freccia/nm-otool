/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:41:26 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/07 22:39:00 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		output_32(struct symtab_command *symc, char *ptr, t_sections sects)
{
	t_strtab		*slist;
	struct nlist	*symtab;
	char			*strtab;
	int				i;

	slist = NULL;
	symtab = (struct nlist *)((char*)ptr + symc->symoff);
	strtab = (char *)ptr + symc->stroff;
	i = 0;
	while (i < (int)symc->nsyms)
	{
		if (list_push(&slist, (struct nlist_64*)&symtab[i], strtab) == EXIT_FAILURE)
		{
			free_list(slist);
			return (EXIT_FAILURE);
		}
		++i;
	}
	insertion_sort(&slist);
	print_list_32(slist, sects);
	free_list(slist);
	return (EXIT_SUCCESS);
}

static uint32_t	init_section(t_sections *sects, uint32_t nsects, 
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

static void	fill_sections(t_sections *sects, struct load_command *lc, uint32_t ncmds)
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
			j += init_section(sects, segcomm->nsects, section, j);
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
}

int			handle_32(void *ptr)
{
	struct mach_header	*header;
	struct load_command		*lc;
	struct symtab_command	*symc;
	t_sections				sects;
	uint32_t				i;

	header = (struct mach_header *)ptr;
	lc = ptr + sizeof(*header);
	i = 0;
	ft_bzero((void*)&sects, sizeof(t_sections));
	fill_sections(&sects, lc, header->ncmds);
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symc = (struct symtab_command *)lc;
			if (output_32(symc, ptr, sects) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			break ;
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}
