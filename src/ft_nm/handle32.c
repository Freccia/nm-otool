/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:41:26 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/12 19:00:01 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		output_32(struct symtab_command *symc, void *ptr,
		t_sections sects)
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
		if (list_push(&slist, (struct nlist_64*)&symtab[i], strtab)
				== EXIT_FAILURE)
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

int				nm_handle_32(void *ptr)
{
	struct mach_header		*header;
	struct load_command		*lc;
	struct symtab_command	*symc;
	t_sections				sects;
	uint32_t				i;

	header = (struct mach_header *)ptr;
	lc = ptr + sizeof(*header);
	i = 0;
	ft_bzero((void*)&sects, sizeof(t_sections));
	fill_sections_32(&sects, lc, header->ncmds);
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
