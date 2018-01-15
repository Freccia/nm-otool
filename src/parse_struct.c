/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 11:20:47 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/15 18:32:27 by lfabbro          ###   ########.fr       */
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
		if (tot > size)
			return (EXIT_FAILURE);
		if ((size_t)sect->offset > size)
			return (EXIT_FAILURE);
		++k;
		sect = (struct section *)((uint8_t *)sect + sizeof(struct section));
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
		if (tot > size)
			return (EXIT_FAILURE);
		if ((size_t)sect->offset > size)
			return (EXIT_FAILURE);
		++k;
		sect = (struct section_64*)((uint8_t*)sect + sizeof(struct section_64));
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

int				parse_load_commands(size_t size, uint32_t ncmds,
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
		if (parse_load_commands_bis(size, lc, tot))
			return (EXIT_FAILURE);
		tot += (size_t)lc->cmdsize;
		if (tot > size)
			return (EXIT_FAILURE);
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}
