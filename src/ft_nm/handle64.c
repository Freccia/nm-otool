/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:40:37 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/26 18:05:01 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		output_64(struct symtab_command *symc, void *ptr,
		t_sections sects)
{
	t_strtab		*slist;
	struct nlist_64	*symtab;
	char			*strtab;
	int				i;

	slist = NULL;
	symtab = (struct nlist_64 *)((char *)ptr + symc->symoff);
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

int				nm_handle_64(void *ptr)
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
	fill_sections_64(&sects, lc, header->ncmds);
	while (i < header->ncmds)
	{
		ft_printf("LC: %d   %p\n", lc->cmd, lc);
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
