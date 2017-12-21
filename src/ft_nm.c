/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 18:04:12 by lfabbro           #+#    #+#             */
/*   Updated: 2017/12/21 20:55:24 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include "libft.h"

#include <stdio.h>  //norme
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>

int			error(char *str)
{	
	ft_printfd(2, "%s\n", str);
	return (EXIT_FAILURE);
}

int			usage(char *name)
{
	ft_printfd(2, "Usage: %s /path/to/exec\n", name);
	return (EXIT_FAILURE);
}

void		print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*strtable;
	struct nlist_64	*array;

	array = (void*)ptr + symoff;
	strtable = (void*)ptr + stroff;
	i = 0;
	while (i < nsyms)
	{
		ft_printf("%s\n", strtable + array[i].n_un.n_strx);
		++i;
	}
}

void		handle_64(void *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	uint32_t				ncmds;
	uint32_t				i;

	header = (struct mach_header_64 *) ptr;
	ncmds = header->ncmds;
	lc = ptr + sizeof(*header);
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *) lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		//lc = (struct load_command*)((uint8_t *) lc + lc->cmdsize);
		lc = (void*) lc + lc->cmdsize;
		++i;
	}
}

void		ft_nm(char *ptr)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t*)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
	}
}

int			main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (ac < 2)
		return (usage(av[0]));
	if ((fd = open(av[1], O_RDONLY)) < 0)
		return (error("fd opening failed"));
	if (fstat(fd, &buf) < 0)
		return (error("fstat failed"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (error("mmap failed"));
	ft_nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
		return (error("munmap failed"));
}
