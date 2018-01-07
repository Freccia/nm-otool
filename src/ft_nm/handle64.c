/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:40:37 by lfabbro           #+#    #+#             */
/*   Updated: 2018/01/07 20:38:28 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

//char		symbolic_debugging_entry(uint8_t n_type)
//{
//
//}

char		get_symbol_type_sect(uint16_t n_sect, t_sections sects)
{
	if (n_sect == sects.st_text)
		return ('T');
	else if (n_sect == sects.st_data)
		return ('D');
	else if (n_sect == sects.st_bss)
		return ('B');
	else if (n_sect == sects.st_common)
		return ('C');
	return ('S');
}

char		get_symbol_type(uint8_t n_type, uint16_t n_sect, t_sections sects)
{
	char	ret;

	ret = '-';
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

void		print_list(t_strtab *slist, t_sections sects)
{
	t_strtab	*ptr;
	char		addr[17];
	char		type;

	ptr = slist;
	ft_memset(&addr[0], ' ', 16);
	addr[16] = 0;
	while (ptr)
	{
		type = get_symbol_type(ptr->type, ptr->sect, sects);
		if (ptr->value)
			ft_printf("%016lx %c %s\n", ptr->value, type, ptr->strx);
			//ft_printf("%016lx %c %s --- %x - %x - %x\n", ptr->value, type, ptr->strx, ptr->type, ptr->value, ptr->sect);
		else
			ft_printf("%16c %c %s\n", ' ', type, ptr->strx);
			//ft_printf("%16c %c %s --- %x - %x - %x\n", ' ', type, ptr->strx, ptr->type, ptr->value, ptr->sect);
		ptr = ptr->next;
	}
}

int			output_64(struct symtab_command *symc, char *ptr, t_sections sects)
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
		// TODO TEST IF n_strx is 0 -> null
		//ft_printf("%s\n", strtab + symtab[i].n_un.n_strx);
		if (list_push(&slist, symtab[i], strtab) == EXIT_FAILURE)
		{
			free_list(slist);
			return (EXIT_FAILURE);
		}
		++i;
	}
	insertion_sort(&slist);
	print_list(slist, sects);
	free_list(slist);
	return (EXIT_SUCCESS);
}

void		fill_sections(t_sections *sects, struct load_command *lc, uint32_t ncmds)
{
	struct segment_command_64	*segcomm;
	struct section_64			*section;
	uint32_t					i;
	uint32_t					k;
	uint32_t					j;

	i = 0;
	j = 1;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			segcomm = (struct segment_command_64 *)lc;
			section = (struct section_64 *)((uint8_t*)segcomm + sizeof(struct segment_command_64));
			k = 1;
			while (k <= segcomm->nsects)
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
		}
		lc = (struct load_command *)((uint8_t *)lc + lc->cmdsize);
		++i;
	}
//	ft_printf("TEXT: %d\n", sects->st_text);
//	ft_printf("DATA: %d\n", sects->st_data);
//	ft_printf("BSS: %d\n", sects->st_bss);
//	ft_printf("COMMON: %d\n", sects->st_common);
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
