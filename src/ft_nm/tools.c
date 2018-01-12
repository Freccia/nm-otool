/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/07 20:42:08 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/12 19:30:00 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static char	get_symbol_type_sect(uint16_t n_sect, t_sections sects)
{
//	ft_printf("%x\n", n_sect);
	if (n_sect == sects.st_text)
		return ('T');
	else if (n_sect == sects.st_data)
		return ('D');
	else if (n_sect == sects.st_bss)
		return ('B');
//	else if (n_sect == sects.st_common)
//		return ('C');
	return ('S');
}

char		get_symbol_type(uint8_t n_type, uint16_t n_sect, t_sections sects)
{
	char	ret;

	ret = '-'; // TODO ok?
	if ((n_type & N_TYPE) == N_UNDF || (n_type & N_TYPE) == N_PBUD)
		ret = 'U';
	else if ((n_type & N_TYPE) == N_SECT)
		ret = get_symbol_type_sect(n_sect, sects);
	else if ((n_type & N_TYPE) == N_ABS)
		ret = 'A';
	else if ((n_type & N_TYPE) == N_INDR)
		ret = 'I';
	if ((n_type & N_PEXT) || !(n_type & N_EXT))
		ret = ft_tolower(ret);
	return (ret);
}

void		print_list_32(t_strtab *slist, t_sections sects)
{
	t_strtab	*ptr;
	char		type;

	ptr = slist;
	while (ptr)
	{
		type = get_symbol_type(ptr->type, ptr->sect, sects);
		if (type != 'U' && type != 'u')
			ft_printf("%08x %c %s\n", (uint32_t)ptr->value, type, ptr->strx);
		else
			ft_printf("%8c %c %s\n", ' ', type, ptr->strx);
		ptr = ptr->next;
	}
}

void		print_list_64(t_strtab *slist, t_sections sects)
{
	t_strtab	*ptr;
	char		type;

	ptr = slist;
	while (ptr)
	{
		type = get_symbol_type(ptr->type, ptr->sect, sects);
		if (type != 'U' && type != 'u')
			//ft_printf("%016lx %c %s -- %x\n", ptr->value, type, ptr->strx, ptr->sect);
			ft_printf("%016lx %c %s\n", ptr->value, type, ptr->strx);
		else
			//ft_printf("%16c %c %s -- %x\n", ' ', type, ptr->strx);
			ft_printf("%16c %c %s\n", ' ', type, ptr->strx, ptr->sect);
		ptr = ptr->next;
	}
}
