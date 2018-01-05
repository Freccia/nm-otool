/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:40:37 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/05 18:52:49 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_strtab	*new_link(t_strtab *next, char *str, uint8_t n_type, uint64_t n_value)
{
	t_strtab	*new;

	if ((new = malloc(sizeof(t_strtab))) == NULL)
		return (NULL);
	new->next = next;
	new->symbol = ft_strdup(str);
	new->type = n_type;
	new->value = n_value;
	return (new);
}

int			quicksort_slist(t_strtab **slist)
{
	t_strtab	*ptr;
	t_strtab	*tmp;

	ptr = *slist;
	while (ptr)
	{
		ptr2 = *slist
		while (ptr2)
		{

		}
	}
}

int			insert_in_list(t_strtab **slist, char *strx, uint8_t n_type, uint64_t n_value)
{
	t_strtab	*new;
	t_strtab	*tmp;

	tmp = *slist;
	if ((new = new_link(NULL, strx, n_type, n_value)) == NULL)
		return (EXIT_FAILURE);
	if (tmp == NULL)
	{
		*slist = new;
		free(tmp);
		return (EXIT_SUCCESS);
	}
	*slist = new;
	new->next = tmp;
	return (EXIT_SUCCESS);
}

//char		symbolic_debugging_entry(uint8_t n_type)
//{
//
//}

char		get_symbol_type(uint8_t n_type)
{
	if (n_type & N_STAB)
		return ('S');
//		return (symbolic_debugging_entry(n_type));
	if ((n_type & N_TYPE) == N_UNDF)
		return ('U');
	if ((n_type & N_TYPE) == N_ABS)
		return ('A');
	if ((n_type & N_TYPE) == N_SECT)
		return ('T');
		//return ('D');
		//return ('B');
	if ((n_type & N_TYPE) == N_PBUD)
		return ('C');
		//return ('S');
	if ((n_type & N_TYPE) == N_INDR)
		return ('I');
	return ('U');
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
		type = get_symbol_type(ptr->type);
		if (ptr->value)
			ft_printf("%016lx %c %s\n", ptr->value, type, ptr->symbol);
		else
			ft_printf("%s %c %s\n", addr, type, ptr->symbol);
		ptr = ptr->next;
	}
}

int			output_64(int nsyms, int symoff, int stroff, char *ptr, t_strtab *slist)
{
	int				i;
	char			*strtable;
	struct nlist_64	*symtab;

	symtab = (struct nlist_64*)((char*)ptr + symoff);
	strtable = (char*)ptr + stroff;
	i = 0;
	while (i < nsyms)
	{
		// TODO TEST IF n_strx is 0 -> null
		//ft_printf("%s\n", strtable + symtab[i].n_un.n_strx);
		//if (ft_strncmp(strtable + symtab[i].n_un.n_strx, "radr://", 7))
		if (insert_in_list(&slist, strtable + symtab[i].n_un.n_strx, symtab[i].n_type, symtab[i].n_value) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		++i;
	}
	print_list(slist);
	return (EXIT_SUCCESS);
}

int			handle_64(void *ptr)
{
	t_strtab				*slist;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	uint32_t				ncmds;
	uint32_t				i;

	slist = NULL;
	header = (struct mach_header_64 *) ptr;
	ncmds = header->ncmds;
	lc = ptr + sizeof(*header);
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *) lc;
			if (output_64(sym->nsyms, sym->symoff, sym->stroff, ptr, slist) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			break ;
		}
		lc = (struct load_command*)((uint8_t *) lc + lc->cmdsize);
		//lc = (void*) lc + lc->cmdsize;
		++i;
	}
	return (EXIT_SUCCESS);
}
