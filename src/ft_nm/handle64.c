/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:40:37 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/06 20:12:22 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

//char		symbolic_debugging_entry(uint8_t n_type)
//{
//
//}

char		get_symbol_type(uint8_t n_type, uint16_t n_sect)
{
	char	ret;

	ret = '-';
//	if (n_type & N_STAB)
//		return ('S');
//		return (symbolic_debugging_entry(n_type));
	if ((n_type & N_TYPE) == N_UNDF)
		ret = 'U';
	if ((n_type & N_TYPE) == N_ABS)
		ret = 'A';
	if ((n_type & N_TYPE) == N_SECT)
	{
		if (n_sect & 0x4)
			ret = 'S';
		if (n_type & N_EXT)
			ret = 'T';
	}
		//return ('D');
		//return ('B');
	if ((n_type & N_TYPE) == N_PBUD)
		ret = 'C';
		//return ('S');
	if ((n_type & N_TYPE) == N_INDR)
		ret = 'I';
	if (n_type & N_PEXT || !(n_type & N_EXT))
		ret = ft_tolower(ret);
	return (ret);
}

void		print_list(t_strtab *slist)
{
	t_strtab	*ptr;
	char		addr[17];
	char		type;

	ptr = slist;
	ft_memset(&addr[0], ' ', 16);
	addr[16] = 0;
	while (ptr)
	{
		type = get_symbol_type(ptr->type, ptr->sect);
		if (ptr->value)
			//ft_printf("%016lx %c %s\n", ptr->value, type, ptr->strx);
			ft_printf("%016lx %c %s --- %x - %x - %p - %x\n", ptr->value, type, ptr->strx, ptr->type, ptr->value, ptr->desc, ptr->sect);
		else
			//ft_printf("%16c %c %s\n", ' ', type, ptr->strx);
			ft_printf("%16c %c %s --- %x - %x - %p - %x\n", ' ', type, ptr->strx, ptr->type, ptr->value, ptr->desc, ptr->sect);
		ptr = ptr->next;
	}
}

int			output_64(struct symtab_command *symc, char *ptr)
{
	t_strtab		*slist;
	struct nlist_64	*symtab;
	char			*strtab;
	int				i;

	symtab = (struct nlist_64 *)((char*)ptr + symc->symoff);
	strtab = (char *)ptr + symc->stroff;
	i = 0;
	while (i < (int)symc->nsyms)
	{
		// TODO TEST IF n_strx is 0 -> null
		//ft_printf("%s\n", strtab + symtab[i].n_un.n_strx);
		if (list_push(&slist, symtab[i], strtab) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		++i;
	}
	insertion_sort(&slist);
	print_list(slist);
	return (EXIT_SUCCESS);
}

int			handle_64(void *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*symc;
	uint32_t				i;

	header = (struct mach_header_64 *)ptr;
	lc = ptr + sizeof(*header);
	i = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symc = (struct symtab_command *)lc;
			if (output_64(symc, ptr) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			break ;
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}
