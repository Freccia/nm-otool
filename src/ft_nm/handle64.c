/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:40:37 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/07 22:38:55 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		output_64(struct symtab_command *symc, char *ptr, t_sections sects)
{
	t_strtab		*slist;
	struct nlist_64	*symtab;
	char			*strtab;
	int				i;

	slist = NULL;
	symtab = (struct nlist_64 *)((char*)ptr + symc->symoff);
	strtab = (char *)ptr + symc->stroff;
	i = 0;
	while (i < (int)symc->nsyms)
	{
		if (list_push(&slist, &symtab[i], strtab) == EXIT_FAILURE)
		{
			free_list(slist);
			return (EXIT_FAILURE);
		}
		++i;
	}
	insertion_sort(&slist);
	print_list_64(slist, sects);
	free_list(slist);
	return (EXIT_SUCCESS);
}

static uint32_t	init_section(t_sections *sects, uint32_t nsects, 
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

static void	fill_sections(t_sections *sects, struct load_command *lc, uint32_t ncmds)
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
			j += init_section(sects, segcomm->nsects, section, j);
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
}

int			handle_64(void *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*symc;
	t_sections				sects;
	uint32_t				i;

	header = (struct mach_header_64 *)ptr;
	lc = ptr + sizeof(*header);
	i = 0;
	ft_bzero((void*)&sects, sizeof(t_sections));
	fill_sections(&sects, lc, header->ncmds);
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symc = (struct symtab_command *)lc;
			if (output_64(symc, ptr, sects) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			break ;
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}
