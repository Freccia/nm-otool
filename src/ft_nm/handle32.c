/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:41:26 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/04 23:05:25 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		output_32(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*strtable;
	struct nlist	*symtab;

	symtab = (struct nlist *)((char*)ptr + symoff);
	strtable = (char*)ptr + stroff;
	i = 0;
	while (i < nsyms)
	{
		ft_printf("%s\n", strtable + symtab[i].n_un.n_strx);
		++i;
	}
}

int			handle_32(void *ptr)
{
	struct mach_header		*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	uint32_t				ncmds;
	uint32_t				i;

	header = (struct mach_header *) ptr;
	ncmds = header->ncmds;
	lc = ptr + sizeof(*header);
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *) lc;
			output_32(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (struct load_command*)((uint8_t *) lc + lc->cmdsize);
		//lc = (void*) lc + lc->cmdsize;
		++i;
	}
	return (EXIT_SUCCESS);
}
