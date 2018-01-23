/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_archive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 17:09:05 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/23 14:53:45 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
**	ar_name == '#1/20' || ar_name == '#1/28'
*/

int			get_object_offset(struct ar_hdr *ar)
{
	int		off;

	off = ft_atoi(&ar->ar_name[3]) + ARCHIVE_HEADER_SIZE;
	return (off);
}

static int	ft_parse_binary_archive(void *ptr, size_t size)
{
	struct mach_header		*header;
	struct load_command		*lc;

	header = (struct mach_header *)ptr;
	lc = ptr + sizeof(*header);
	if (SUPPORTED_FAT(header->magic))
		return (parse_fat(ptr, size));
	if (SUPPORTED_ARCH(header->magic))
		return (parse_load_commands(size, header->ncmds, lc));
	else if (header->magic == FT_ARMAG)
	{
		ft_printf("Binary error.");
		return (EXIT_FAILURE);
	}
	else
	{
		ft_printf("Binary file not supported.\n");
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

static int	parse_archive_bis(void *ptr, size_t size,
		t_archive ar, t_quad q)
{
	while (q.tot < ar.symtab_size)
	{
		if (q.tmp != ar.symtab[q.i])
		{
			ar.ar_obj = (struct ar_hdr *)((uint8_t *)ptr + ar.symtab[q.i]);
			if (ft_strncmp(ar.ar_obj->ar_fmag, ARFMAG, 2))
				return (EXIT_FAILURE);
			if ((q.off = get_object_offset(ar.ar_obj)) == (uint32_t)-1)
				return (EXIT_FAILURE);
			q.tmp = ar.symtab[q.i];
			if (ft_parse_binary_archive((void *)ar.ar_obj + q.off, size))
				return (EXIT_FAILURE);
		}
		q.tot += sizeof(q.i) * 2;
		if (q.tot > size + ar.symtab_offset)
			return (EXIT_FAILURE);
		q.i += 2;
	}
	return (EXIT_SUCCESS);
}

/*
**	ARFMAG -> consistency check
*/

int			parse_archive(void *ptr, size_t size)
{
	t_archive		ar;
	t_quad			q;

	ar.ar = (struct ar_hdr *)(ptr + ARMAG_LEN);
	ar.ar_obj = NULL;
	if (ft_strncmp(ar.ar->ar_fmag, ARFMAG, 2))
		return (EXIT_FAILURE);
	if ((ar.symtab_offset = get_object_offset(ar.ar)) == (uint32_t)-1)
		return (EXIT_FAILURE);
	ar.symtab_size = *(uint32_t *)(ptr + ar.symtab_offset + ARMAG_LEN);
	ar.symtab = (uint32_t *)(ptr + ar.symtab_offset + ARMAG_LEN + sizeof(int));
	q.tot = 0;
	q.tmp = -1;
	q.i = 1;
	return (parse_archive_bis(ptr, size, ar, q));
}
