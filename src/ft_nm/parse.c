/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/10 20:04:20 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/10 20:32:50 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		parse_symtab_command(size_t size,
		struct symtab_command *symc)
{
	size_t			tot;
	int				i;

	if (symc->stroff > size)
		return (EXIT_FAILURE);
	tot = 0;
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

static int		parse_load_commands(size_t size, uint32_t ncmds,
		struct load_command *lc)
{
	struct symtab_command	*symc;
	size_t					tot;
	uint32_t				i;

	tot = 0;
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symc = (struct symtab_command *)lc;
			if (parse_symtab_command(size, symc))
				return (EXIT_FAILURE);
		}
		tot += (size_t)lc->cmdsize;
		if (tot > size)
			return (EXIT_FAILURE);
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}

int		ft_nm_parse(void *ptr, size_t size)
{
	struct mach_header		*header;
	struct load_command		*lc;

	header = (struct mach_header *)ptr;
	lc = ptr + sizeof(*header);
	if (parse_load_commands(size, header->ncmds, lc))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
